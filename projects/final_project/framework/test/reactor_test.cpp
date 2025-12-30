
#include <iostream>
#include <unistd.h>     // for pipe, read, write
#include <fcntl.h>      // for fcntl
#include <cstring>

#include "reactor.hpp"
#include "linux_listener.hpp"
#include "logger.hpp"
#include "handleton.hpp"

using namespace ilrd;
//using namespace reactor_details;

#define RED ("\033[31m")
#define GREEN ("\033[32m")
#define BLUE ("\x1B[34m")
#define DEFAULT ("\033[0m")

void BasicRead(int fd)
{
    char buffer[128];
    ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes > 0)
    {
        buffer[bytes] = '\0';
        std::cout << "Read: " << buffer << std::endl;
    }
}

void BasicWrite(int fd)
{
    const char* msg = "Pong\n";
    write(fd, msg, strlen(msg));
    std::cout << "Write: Ping\n";
}

void TestPingPongAndStop()
{
    std::cout << BLUE << "Test Ping Pong And Stop" << DEFAULT << std::endl;
    LinuxListener listener;
    Reactor reactor(&listener);

    int pipefd1[2];
    pipe(pipefd1);

    int pipe_read = pipefd1[0];
    int pipe_write = pipefd1[1];

    auto start_time = std::chrono::steady_clock::now();

    reactor.Register(pipe_read, details::Mode::READ, [&reactor, start_time, pipe_read](){
        BasicRead(pipe_read);
        sleep(1);

        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start_time);

        if (duration.count() >= 5)
        {
            reactor.Stop(); // stop the loop after 5 seconds
        }
    });

    reactor.Register(pipe_write, details::Mode::WRITE, [pipe_write](){
        BasicWrite(pipe_write);
    });

    reactor.Run();
}

void TestPingPongRegisterUnregister()
{
    std::cout << BLUE << "Test Register and Unregister" << DEFAULT << std::endl;

    LinuxListener listener;
    Reactor reactor(&listener);

    int pipefd[2];
    pipe(pipefd);

    int pipe_read = pipefd[0];
    int pipe_write = pipefd[1];

    auto start_time = std::chrono::steady_clock::now();

    // Define write callback
    std::function<void()> write_callback;

    // Define read callback
    std::function<void()> read_callback;

    /* & - capture all local variables */
    write_callback = [&]() {
        BasicWrite(pipe_write);

        // Unregister write
        reactor.Unregister(pipe_write, details::Mode::WRITE);

        // Register read
        reactor.Register(pipe_read, details::Mode::READ, read_callback);
    };

    read_callback = [&]() {
        BasicRead(pipe_read);
        sleep(1);
        
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start_time);
        
        if (duration.count() >= 5)
        {
            reactor.Stop();
            return;
        }
        
        // Unregister read
        reactor.Unregister(pipe_read, details::Mode::READ);
        
        // Register write
        reactor.Register(pipe_write, details::Mode::WRITE, write_callback);
    };

    // Start with a write
    reactor.Register(pipe_write, details::Mode::WRITE, write_callback);

    reactor.Run();
}

int main()
{
    TestPingPongAndStop();
    TestPingPongRegisterUnregister();
    return (0);
}
