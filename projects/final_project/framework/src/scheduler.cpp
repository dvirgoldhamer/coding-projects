
/*name: Dvir Goldhamer

File Name: scheduler.cpp

purpose: implementation for a scheduler

reviewed by Ofir Hasson

Date: 11.08.25*/

#include "scheduler.hpp"
#include "logger.hpp"

namespace ilrd
{

Scheduler::Scheduler() 
{
    struct sigevent sev = {};
    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = TimerHandler;
    sev.sigev_value.sival_ptr = this;  
    sev.sigev_notify_attributes = nullptr;

    if (timer_create(CLOCK_MONOTONIC, &sev, &m_timer) == -1)
    {
        throw std::runtime_error("scheduler creation failed");
    }
    
}

Scheduler::~Scheduler()
{
    timer_delete(m_timer);
}

void Scheduler::AddTask(std::shared_ptr<ISchedTask> task,std::chrono::milliseconds task_when)
{
    auto now_milli = Now();
    
    TaskItem item(task, now_milli + task_when);
    std::unique_lock<std::mutex> lock(m_mtx);
    bool update_needed = m_pq.empty() || (item.m_when < m_pq.top().m_when);
    m_pq.push(item);
    
    if (update_needed)
    {        
        
        SetNextFire(this);
    }
}

void Scheduler::TimerHandler(union sigval arg)
{
    Scheduler* sched = static_cast<Scheduler*>(arg.sival_ptr);
    TaskItem item, next_item;
    
    {
        std::unique_lock<std::mutex> lock(sched->m_mtx);
        
        item = sched->m_pq.top();
        sched->m_pq.pop();
        if (!sched->m_pq.empty())
        {            
            SetNextFire(sched);
        }
    }

    try
    {
        item.m_task->Run();
    }
    catch(const std::exception& e)
    {
        Handleton<Logger>::GetInstance()->Log(e.what(), Logger::ERROR);
    }
}

bool Scheduler::TaskItem::operator<(const TaskItem& item) const
{
    return m_when > item.m_when;
}

std::chrono::milliseconds Scheduler::Now()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

void Scheduler::SetNextFire(Scheduler* sched)
{
    auto now_milli = Now();
    
    std::chrono::milliseconds item_when = sched->m_pq.top().m_when;
    
    auto diff = (item_when > now_milli) ? (item_when - now_milli) : std::chrono::milliseconds(1);
    
    struct itimerspec itime = {};
    itime.it_value.tv_sec = (diff.count()) / 1000;
    itime.it_value.tv_nsec = (diff.count() % 1000) * 1000000;
    
    if (timer_settime(sched->m_timer, 0, &itime, nullptr) < 0)
    {
        Handleton<Logger>::GetInstance()->Log("update timer failed", Logger::ERROR);
    }
}

}//namespace ilrd