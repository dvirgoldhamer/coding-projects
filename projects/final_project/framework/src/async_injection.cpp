
/*name: Dvir Goldhamer

File Name: async_injection.cpp

purpose: implementation for async_injection

reviewed by

Date: */

#include "async_injection.hpp"

namespace ilrd
{

AsyncInjection::AsyncInjection(std::function<bool()> func, std::chrono::milliseconds when) : m_func(func), m_when(when)
{
    ScheduleTask();
}

void AsyncInjection::ScheduleTask()
{
    std::shared_ptr<AsyncInjectionTask> task(new AsyncInjectionTask(*this)); 

    Handleton<Scheduler>::GetInstance()->AddTask(task, m_when);
}   

void AsyncInjection::PerformAction()
{
    bool flag = m_func();
    if (flag)
    {
        ScheduleTask();
    }   
    else 
    {
        delete this;
    }
}

AsyncInjection::AsyncInjectionTask::AsyncInjectionTask(AsyncInjection& async_injection) : m_async_injection(async_injection)
{
}

void AsyncInjection::AsyncInjectionTask::Run()
{
    m_async_injection.PerformAction();
}

}