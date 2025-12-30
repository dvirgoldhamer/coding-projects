/*name: Dvir Goldhamer

File Name: reactor.hpp

purpose: API for a reactor

reviewed by

Date: */

#ifndef IRLD_RD168_169__REACTOR_HPP
#define IRLD_RD168_169__REACTOR_HPP

#include <functional> //std::function
#include <map>
#include <vector>
#include <memory> //shared_pointer

namespace ilrd
{

namespace details
{

enum Mode
{
    READ,
    WRITE
};

}

class IListener
{
public: 
    virtual std::vector<std::pair<int, details::Mode>> Listen(std::vector<std::pair<int,details::Mode>>& fds) = 0; 

}; //class IListener

class Reactor
{
public:
  
    explicit Reactor(std::shared_ptr<IListener> listener);
    Reactor(const Reactor& other) = delete;
    Reactor& operator=(const Reactor& other) = delete;
    ~Reactor() = default;

    void Register(int fd, details::Mode mode, std::function<void()> callback); 
    void Unregister(int fd, details::Mode mode);
    void Run() noexcept; //Run is a blocking call
    void Stop(); //To stop reactor, register this function as a callback.
    //stoppage of the reactor will only happen when all of the modified file descriptors' callbacks at a specific time were executed. 

private:
   std::shared_ptr<IListener> m_listener;
    std::map<std::pair<int,details::Mode>,std::function<void()>> m_callbacks; 
    std::vector<std::pair<int, details::Mode>> m_fds;

    bool m_is_running;
}; //class Reactor

} //namespace ilrd
#endif //IRLD_RD168_169__REACTOR_HPP
