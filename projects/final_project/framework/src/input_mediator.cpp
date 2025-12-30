/*name: Dvir Goldhamer

File Name: input_mediator.cpp

purpose: Implementation for a input mediator

reviewed by Denis Charenko: 

Date: 17.08.25*/

#include "input_mediator.hpp"
#include "thread_pool.hpp"
#include "handleton.hpp"
#include "cpf_thread.hpp"

namespace ilrd
{

void InputMediator::Run(int fd, Mode mode, std::shared_ptr<interfaces::IInputProxy> input_proxy)

{
    std::shared_ptr<interfaces::IKeyArgs> key_args = input_proxy->GetKeyArgs(fd, mode);
    
    if (key_args == nullptr)
    {
        return;
    }
    
    std::shared_ptr<CpfThread> cThreadPtr = std::make_shared<CpfThread>(key_args);
    Handleton<ThreadPool>::GetInstance()->AddTask(cThreadPtr, ThreadPool::Priority::HIGH);
}

}//namespace ilrd
