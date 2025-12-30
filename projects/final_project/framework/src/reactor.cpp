/*name: Dvir Goldhamer

File Name: reactor.hpp

purpose: implementation for a reactor

reviewed by

Date: */

#include <iostream> // cerr

#include "reactor.hpp"


namespace ilrd
{

Reactor::Reactor(std::shared_ptr<IListener> listener) : m_listener(listener), m_is_running(false)
{
}

void Reactor::Register(int fd, details::Mode mode, std::function<void()> callback)
{
    m_callbacks[{fd, mode}] = callback;
    m_fds.emplace_back(fd, mode);
} 

void Reactor::Unregister(int fd, details::Mode mode)
{
    m_callbacks.erase({fd, mode});
    
    for (auto it = m_fds.begin(); it != m_fds.end(); ++it)
    {
        if (it->first == fd)
        {
            m_fds.erase(it);
            break; 
        }
    }
} 

void Reactor::Run() noexcept
{ 
    m_is_running = true;

    while(m_is_running)
    {
        std::vector<std::pair<int, details::Mode>> modified_fds;
        try
        {
          modified_fds = m_listener->Listen(m_fds);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            continue;
        }
        

        std::map<std::pair<int,details::Mode>,std::function<void()>> callbacks_copy(m_callbacks); 

        for (auto const& pair : modified_fds)
        {
            auto it = callbacks_copy.find(pair);
            //find is assumed to always find the correct pair
            try
            {
                it->second();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }  
        }

    }
}

void Reactor::Stop()
{
    m_is_running = false;
}

}