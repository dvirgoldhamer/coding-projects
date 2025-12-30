/*name: Dvir Goldhamer

File Name: linux_listener.hpp

purpose: API for a linux_listener

reviewed by

Date: */

#ifndef IRLD_RD168_169_LINUX_LISTENER_HPP
#define IRLD_RD168_169_LINUX_LISTENER_HPP

#include "reactor.hpp"

namespace ilrd
{

class LinuxListener : public IListener
{
public:
    virtual std::vector<std::pair<int, details::Mode>> Listen(std::vector<std::pair<int,details::Mode>>& fds); 

private:
    void SetWorkingSets(std::vector<std::pair<int,details::Mode>>& fds, fd_set& reading_set, fd_set& writing_set, size_t size, int& max_fd);
};

}//namespace ilrd
#endif //IRLD_RD168_169_LINUX_LISTENER_HPP