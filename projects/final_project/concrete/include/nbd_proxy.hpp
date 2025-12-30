/*name: Dvir Goldhamer

File Name: nbd_proxy.hpp

purpose: API for NBDProxy

reviewed by : 

Date: */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#ifndef IRLD_RD168_169_NBD_PROXY_HPP
#define IRLD_RD168_169_NBD_PROXY_HPP

#include <memory>

#include "framework_interfaces.hpp"
#include "key_args.hpp"

#include <assert.h>
#include <errno.h>
#include <err.h>
#include <fcntl.h>
#include <linux/nbd.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>

#include "buse.h"
#include "handleton.hpp"
#include "concrete_utils.hpp"

#ifndef BUSE_DEBUG
  #define BUSE_DEBUG (0)
#endif

namespace ilrd
{
    
class NBDProxy : public interfaces::IInputProxy
{
    public:
    std::shared_ptr<interfaces::IKeyArgs> GetKeyArgs(int fd, ilrd::Mode mode);

};

class NBDHandler /*Separate class, or inside NBDProxy itself*/
{
public:

    int Init();
    
    std::shared_ptr<interfaces::IKeyArgs> HandleReadWrite();
    
    // Called from ResponseManager / AsyncInjection
    void SendResponse(__be64 id, STATUS status, char* read_buffer = nullptr, u_int64_t buffer_size = 0);
    
    int GetSocketFd();

    static const u_int64_t MINIONS_AMOUNT;
    static const u_int64_t NBD_SIZE;
    
private:

    int m_sockfd;
    friend class Handleton<NBDHandler>;
};

} //namespace ilrd

#endif //IRLD_RD168_169_NBD_PROXY_HPP