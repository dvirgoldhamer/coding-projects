/*name: Dvir Goldhamer

File Name: framework.cpp

purpose: Implementation for a framework prototype

reviewed by Denis Charenko: 

Date: 17.08.25*/

#include "framework_interfaces.hpp"
#include "framework.hpp"
#include "linux_listener.hpp"

namespace ilrd
{

Framework::Framework(const std::unordered_map<FdModeWrapper, std::shared_ptr <interfaces::IInputProxy>, FdModeHash>& proxy_map, 

const std::unordered_map<int, std::function<std::shared_ptr<interfaces::ICommand>()>>& creators,

const std::string& plugins_dir) : 

m_reactor(std::make_shared<LinuxListener>()), m_dirMonitor(plugins_dir), m_callback(m_dllLoader,&DllLoader::Load,nullptr)
{
    m_dirMonitor.Subscribe(&m_callback);

    factory_details::Factory<int, interfaces::ICommand>* factory = Handleton<factory_details::Factory<int, interfaces::ICommand>>::GetInstance();

    //register creators to factory
    for (auto& it : creators)
    {
        factory->Register(it.first, it.second);
    }

    //register file descriptors to be monitored by the reactor
    for (auto& it : proxy_map)
    {
        auto fd = it.first.m_fd;
        auto mode = it.first.m_mode;
        auto proxy = it.second;

        m_reactor.Register(it.first.m_fd, static_cast<details::Mode>(it.first.m_mode), [fd, mode, proxy]()
        {
            InputMediator med;
            med.Run(fd, mode, proxy);
        });
    }
}

void Framework::Run()
{
    m_dirMonitor.Run();    
    m_reactor.Run();
}

}//namespace ilrd
