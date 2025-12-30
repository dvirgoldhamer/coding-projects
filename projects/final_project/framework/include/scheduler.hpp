/*name: Dvir Goldhamer

File Name: scheduler.hpp

purpose: API for a scheduler

reviewed by Ofir Hasson

Date: 11.08.25*/

#ifndef IRLD_RD168_169_SCHEDULER_HPP
#define IRLD_RD168_169_SCHEDULER_HPP

#include <chrono>

#include <signal.h> //struct sigevent
#include <queue>
#include <mutex>
#include <memory>

#include "handleton.hpp"

namespace ilrd
{

class Scheduler
{
public:

    class ISchedTask
    {
    public:
        virtual void Run() = 0;
    };
    
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;

    void AddTask(std::shared_ptr<ISchedTask> task,std::chrono::milliseconds when); //when represents how long from now the task needs to be executed in milliseconds. argument must be a positive number. 

    private:
    
    explicit Scheduler(); //throws std::runtime_error
    ~Scheduler();
    
    struct TaskItem
    {
        std::shared_ptr<ISchedTask> m_task; 
        std::chrono::milliseconds m_when;
        inline bool operator<(const TaskItem& item) const;
    };

    timer_t m_timer;

    std::priority_queue<TaskItem> m_pq;
    std::mutex m_mtx;
    
    static void TimerHandler(union sigval arg);
    static std::chrono::milliseconds Now();
    static void SetNextFire(Scheduler* sched);

    friend class Handleton<Scheduler>;

}; //class Scheduler

}//namespace ilrd
#endif //IRLD_RD168_169_SCHEDULER_HPP

