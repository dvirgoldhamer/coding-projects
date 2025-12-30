/*name: Dvir Goldhamer

File Name: nbd_handler.cpp

purpose: Implementation for a repsponse proxy class

reviewed by :

Date: */

#include <iostream>

#include "key_args.hpp"
#include "nbd_proxy.hpp"
#include "raid_manager.hpp"

namespace ilrd
{

const u_int64_t NBDHandler::MINIONS_AMOUNT = 3;
const u_int64_t NBDHandler::NBD_SIZE = RAIDManager::MINION_SIZE * MINIONS_AMOUNT;


#ifndef BUSE_DEBUG
  #define BUSE_DEBUG (0)
#endif

#ifdef WORDS_BIGENDIAN
u_int64_t ntohll(u_int64_t a) 
{
  return a;
}
#else
u_int64_t ntohll(u_int64_t a) 
{
  u_int32_t lo = a & 0xffffffff;
  u_int32_t hi = a >> 32U;
  lo = ntohl(lo);
  hi = ntohl(hi);
  return ((u_int64_t) lo) << 32U | hi;
}
#endif
#define htonll ntohll

static int read_all(int fd, char* buf, size_t count)
{
  int bytes_read;

  while (count > 0) 
  {
    bytes_read = read(fd, buf, count);
    assert(bytes_read > 0);
    buf += bytes_read;
    count -= bytes_read;
  }
  assert(count == 0);

  return 0;
}

static int write_all(int fd, char* buf, size_t count)
{
  int bytes_written;

  while (count > 0) 
  {
    bytes_written = write(fd, buf, count);
    assert(bytes_written > 0);
    buf += bytes_written;
    count -= bytes_written;
  }
  assert(count == 0);

  return 0;
}

std::shared_ptr<interfaces::IKeyArgs> NBDProxy::GetKeyArgs(int fd, ilrd::Mode mode)
{
    (void)fd;
    (void)mode;

    return Handleton<NBDHandler>::GetInstance()->HandleReadWrite();
}

void NBDHandler::SendResponse(__be64 id, STATUS status, char* read_buffer, uint64_t buffer_size)
{
    struct nbd_reply reply;


        
    reply.error = (status == SUCCESS ? htonl(0) : htonl(EIO));
    reply.magic = htonl(NBD_REPLY_MAGIC);
    reply.cookie = id;
  
    write_all(m_sockfd, (char*)&reply, sizeof(struct nbd_reply));

    if (read_buffer != nullptr)
    {
        std::cout << "Read " << std::endl;
        write_all(m_sockfd, (char*)read_buffer, buffer_size);
    }
    else
    {
        std::cout << "Write response in nbd " << std::endl;
    }

    std::cout << status << std::endl;
    
}

std::shared_ptr<interfaces::IKeyArgs> NBDHandler::HandleReadWrite()
{
    u_int64_t from;
    u_int32_t len;
    ssize_t bytes_read;
    struct nbd_request request;

    bytes_read = read(m_sockfd, &request, sizeof(request)); 
    
    assert(bytes_read == sizeof(request));

    
    len = ntohl(request.len);
    from = ntohll(request.from);
    __be64 cookie = request.cookie;

    assert(request.magic == htonl(NBD_REQUEST_MAGIC));
    
    switch(ntohl(request.type)) 
    {
        /* I may at some point need to deal with the the fact that the
        * official nbd server has a maximum buffer size, and divides up
        * oversized requests into multiple pieces. This applies to reads
        * and writes.
        */
    case NBD_CMD_READ:
       
        if (BUSE_DEBUG) fprintf(stderr, "Request for read of size %d\n", len);
        /* Fill with zero in case actual read is not implemented */
        return std::make_shared<ReadArgs>(len, from, cookie);

    break;
    case NBD_CMD_WRITE:
    {
        if (BUSE_DEBUG) fprintf(stderr, "Request for write of size %d\n", len);
        std::shared_ptr<char[]> chunk = std::make_shared<char[]>(len);

        read_all(m_sockfd, chunk.get(), len);

        return std::make_shared<WriteArgs>(len, from, chunk, cookie);

    break;
    }
    case NBD_CMD_DISC:
        if (BUSE_DEBUG) fprintf(stderr, "Got NBD_CMD_DISC\n");
        /* Handle a disconnect request. */
        return nullptr;

    break;
    
    default:
        struct nbd_reply reply;
        reply.magic = htonl(NBD_REPLY_MAGIC);
        reply.error = htonl(0);
        reply.cookie = request.cookie;
        write_all(m_sockfd, (char*)&reply, sizeof(struct nbd_reply));
    }

    return nullptr;
}

/* Sets signal action like regular sigaction but is suspicious. */
static int set_sigaction(int sig, const struct sigaction * act) 
{
    struct sigaction oact;
    int r = sigaction(sig, act, &oact);
    if (r == 0 && oact.sa_handler != SIG_DFL) 
    {
        warnx("overriden non-default signal handler (%d: %s)", sig, strsignal(sig));
    }
    return r;
}

static int nbd_dev_to_disconnect = -1;
static void disconnect_nbd(int signal) 
{
  (void)signal;
  if (nbd_dev_to_disconnect != -1) 
  {
    if(ioctl(nbd_dev_to_disconnect, NBD_DISCONNECT) == -1) 
    {
      warn("failed to request disconect on nbd device");
    } 
    else 
    {
      nbd_dev_to_disconnect = -1;
      fprintf(stderr, "sucessfuly requested disconnect on nbd device\n");
    }
  }
}

int NBDHandler::GetSocketFd()
{
    return m_sockfd;
}

int NBDHandler::Init()
{
    int sp[2]; //init socketpair    
    int sk, nbd, err, flags;
    
    err = socketpair(AF_UNIX, SOCK_STREAM, 0, sp); //create tcp socket pair
    assert(!err);

    nbd = open("/dev/nbd0" , O_RDWR); //create nbd file descriptor
    if (nbd == -1) 
    {
        fprintf(stderr, 
            "Failed to open `%s': %s\n"
            "Is kernel module `nbd' loaded and you have permissions "
            "to access the device?\n", "/dev/nbd0", strerror(errno));
            return 1;
    }
        
    err = ioctl(nbd, NBD_SET_SIZE, NBD_SIZE); //set size for nbd fd
    assert(err != -1);

    err = ioctl(nbd, NBD_CLEAR_SOCK); //change parameters of nbd file descriptor
    assert(err != -1);
    
    pid_t pid = fork();
    
    /***************if i am the child process*********************************/
    if (pid == 0) 
    {
        
        /* Block all signals to not get interrupted in ioctl(NBD_DO_IT), as
        * it seems there is no good way to handle such interruption.*/
        sigset_t sigset;
        if (sigfillset(&sigset) != 0 || sigprocmask(SIG_SETMASK, &sigset, NULL) != 0) 
        {
            warn("failed to block signals in child");
            exit(EXIT_FAILURE);
        }

        /* The child needs to continue setting things up. */
        close(sp[0]);
        sk = sp[1];
        //leaving the child to work on sp[1], parent is sp[0]

        //setting the socket that the NBD device will use for communication with the NBD server - creating the nbd client

        if(ioctl(nbd, NBD_SET_SOCK, sk) == -1)
        {
            fprintf(stderr, "ioctl(nbd, NBD_SET_SOCK, sk) failed.[%s]\n", strerror(errno));
            exit(EXIT_FAILURE);
        }
        else
        {
            #if defined NBD_SET_FLAGS
            flags = 0;
            #if defined NBD_FLAG_SEND_TRIM
            flags |= NBD_FLAG_SEND_TRIM;
            #endif
            #if defined NBD_FLAG_SEND_FLUSH
            flags |= NBD_FLAG_SEND_FLUSH;
            #endif
            if (flags != 0 && ioctl(nbd, NBD_SET_FLAGS, flags) == -1)
            {
            fprintf(stderr, "ioctl(nbd, NBD_SET_FLAGS, %d) failed.[%s]\n", flags, strerror(errno));
                exit(EXIT_FAILURE);
            }
            #endif
            err = ioctl(nbd, NBD_DO_IT);
            if (BUSE_DEBUG) fprintf(stderr, "nbd device terminated with code %d\n", err);
            if (err == -1) 
            {
                warn("NBD_DO_IT terminated with error");
                exit(EXIT_FAILURE);
            }
        }

        if (ioctl(nbd, NBD_CLEAR_QUE) == -1 ||
            ioctl(nbd, NBD_CLEAR_SOCK) == -1) 
        {
            warn("failed to perform nbd cleanup actions");
            exit(EXIT_FAILURE);
        }

        exit(0);

    } //end of child

    /* Parent handles termination signals by terminating nbd device. */
    assert(nbd_dev_to_disconnect == -1);
    nbd_dev_to_disconnect = nbd;
    struct sigaction act;
    act.sa_handler = disconnect_nbd;
    act.sa_flags = SA_RESTART;
    if (sigemptyset(&act.sa_mask) != 0 ||
        sigaddset(&act.sa_mask, SIGINT) != 0 ||
        sigaddset(&act.sa_mask, SIGTERM) != 0) 
    {
        warn("failed to prepare signal mask in parent");
        return EXIT_FAILURE;
    }

    if (set_sigaction(SIGINT, &act) != 0 || set_sigaction(SIGTERM, &act) != 0)
    {
        warn("failed to register signal handlers in parent");
        return EXIT_FAILURE;
    }

    close(sp[1]); //parent is sp[0]

    m_sockfd = sp[0];

    return EXIT_SUCCESS;

}

} //namespace ilrd
