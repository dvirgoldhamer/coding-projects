/*name: Dvir Goldhamer

File Name: async_injection.hpp

purpose: API for async_injection

reviewed by

Date: */

#ifndef IRLD_RD168_169_ASYNC_INJECTION_HPP
#define IRLD_RD168_169_ASYNC_INJECTION_HPP

#include <functional>

#include "scheduler.hpp"
#include "ilrd_uid.hpp"

namespace ilrd
{

class AsyncInjection
{
public:
    explicit AsyncInjection(std::function<bool()> func, std::chrono::milliseconds when);

private:
    class AsyncInjectionTask : public Scheduler::ISchedTask
    {
    public:
        AsyncInjectionTask(AsyncInjection& async_injection);
        virtual void Run();

    private:
        AsyncInjection& m_async_injection;
    }; // end of AsyncInjectionTask
    
    void PerformAction();
    void ScheduleTask();
    ~AsyncInjection() = default;
    
    std::function<bool()> m_func;
    std::chrono::milliseconds m_when;

}; // end of class AsyncInjection

}//namespace ilrd

#endif //IRLD_RD168_169_ASYNC_INJECTION_HPP

