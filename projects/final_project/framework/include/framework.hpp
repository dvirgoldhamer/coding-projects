/*name: Dvir Goldhamer

File Name: framework.hpp

purpose: API for a framework prototype

reviewed by Denis Charenko: 

Date: 17.08.25*/

#ifndef ILRD_RD168_169_FRAMEWORK_HPP
#define ILRD_RD168_169_FRAMEWORK_HPP

#include <unordered_map>
#include <functional> //std::function

#include "framework_interfaces.hpp"
#include "framework_utils.hpp"
#include "handleton.hpp"
#include "input_mediator.hpp"
#include "factory.hpp"
#include "cpf_thread.hpp"
#include "reactor.hpp"
#include "dir_monitor.hpp"
#include "dll_loader.hpp"

namespace ilrd
{

class Framework
{
public:

    Framework(const std::unordered_map<FdModeWrapper,std::shared_ptr <interfaces::IInputProxy>, FdModeHash>& proxy_map, 
    const std::unordered_map<int, std::function<std::shared_ptr<interfaces::ICommand>()>>& creators, 
    const std::string& plugins_dir);

    void Run();
    
private:
    Reactor m_reactor;
    DirMonitor m_dirMonitor;
    DllLoader m_dllLoader;
    Callback<std::string, DllLoader> m_callback;
};

}//namespace ilrd

#endif //ILRD_RD168_169_FRAMEWORK_HPP
