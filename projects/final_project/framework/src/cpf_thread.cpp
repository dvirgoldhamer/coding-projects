/*name: Dvir Goldhamer

File Name: cpf_thread.cpp

purpose: Implementation for a cpf_thread task

reviewed by Denis Charenko: 

Date: 17.08.25*/

#include "cpf_thread.hpp"
#include "factory.hpp"
#include "async_injection.hpp"
#include "ilrd_uid.hpp"


namespace ilrd
{

CpfThread::CpfThread(std::shared_ptr<interfaces::IKeyArgs> key_args) : m_key_args(key_args)
{
}

void CpfThread::Run()
{
    //create object inside factory with the relevant arguments
    std::shared_ptr<interfaces::ICommand> command = Handleton<factory_details::Factory<int, interfaces::ICommand>>::GetInstance()->CreateObject(m_key_args->GetKey());
    
    //execute command and save result for periodic execution
    std::pair<std::function<bool()>, std::chrono::milliseconds> async_pair = command->Execute(m_key_args);

    if (async_pair.first != nullptr)
    {
        new AsyncInjection(async_pair.first, async_pair.second);
    }
}

}//namespace ilrd
