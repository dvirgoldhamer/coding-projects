/*name: Dvir Goldhamer

File Name: cof_thread.hpp

purpose: API for a cpf_thread task

reviewed by Denis Charenko: 

Date: 17.08.25*/

#ifndef ILRD_RD168_169_CPF_THREAD_HPP
#define ILRD_RD168_169_CPF_THREAD_HPP

#include "thread_pool.hpp"
#include "framework_interfaces.hpp"

namespace ilrd
{

class CpfThread : public ThreadPool::ITPTask
{
public:
    CpfThread(std::shared_ptr<interfaces::IKeyArgs> key_args);
    void Run();

private:
    std::shared_ptr<interfaces::IKeyArgs> m_key_args;
};


}//namespace ilrd

#endif //ILRD_RD168_169_CPF_THREAD_HPP
