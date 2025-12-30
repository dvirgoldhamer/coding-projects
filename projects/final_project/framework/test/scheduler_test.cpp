
#include <iostream>
#include "scheduler.hpp"
#include "handleton.hpp"
#include "logger.hpp"

using namespace ilrd;

#define RED ("\033[31m")
#define GREEN ("\033[32m")
#define BLUE ("\x1B[34m")
#define DEFAULT ("\033[0m")

class MyTask : public Scheduler::ISchedTask
{
    void Run() override
    {
        auto now = std::chrono::system_clock::now();
        std::time_t time_now = std::chrono::system_clock::to_time_t(now);
        std::cout << std::put_time(std::localtime(&time_now), "%Y-%m-%d %H:%M:%S") << std::endl;
    }
};

class ReentrantTask : public Scheduler::ISchedTask
{
    void Run() override
    {
        static bool added = false;
        auto now = std::chrono::system_clock::now();
        std::time_t time_now = std::chrono::system_clock::to_time_t(now);
        std::cout << std::put_time(std::localtime(&time_now), "%Y-%m-%d %H:%M:%S") << std::endl;

        if (!added)
        {
            added = true;
            Scheduler* sched = ilrd::Handleton<Scheduler>::GetInstance();
            std::shared_ptr<MyTask> new_task = std::make_shared<MyTask>();
            sched->AddTask(new_task, static_cast<std::chrono::milliseconds>(2000));
        }
    }
};

void TestSyncScheduler()
{
    std::cout << BLUE << "TestSyncScheduler" << std::endl;
    std::cout << "Should print in 2,4,6 seconds" << DEFAULT << std::endl;

    Scheduler* sched = ilrd::Handleton<Scheduler>::GetInstance();
    std::shared_ptr<MyTask> task_ptr = std::make_shared<MyTask>();
    sched->AddTask(task_ptr, static_cast<std::chrono::milliseconds>(2000));
    sched->AddTask(task_ptr, static_cast<std::chrono::milliseconds>(4000));
    sched->AddTask(task_ptr, static_cast<std::chrono::milliseconds>(6000));
    sleep(7);
}

void TestAnSyncScheduler()
{
    std::cout << BLUE << "TestAnSyncScheduler" << std::endl;
    std::cout << "Should print in 1,2,4 seconds" << DEFAULT << std::endl;

    Scheduler* sched = ilrd::Handleton<Scheduler>::GetInstance();

    std::shared_ptr<MyTask> task_ptr = std::make_shared<MyTask>();
    sched->AddTask(task_ptr,  static_cast<std::chrono::milliseconds>(4000));
    sched->AddTask(task_ptr,  static_cast<std::chrono::milliseconds>(2000));
    sched->AddTask(task_ptr,  static_cast<std::chrono::milliseconds>(1000));
    sleep(5);
}

void TestImmediateScheduler()
{
    std::cout << BLUE << "TestImmediateScheduler" << std::endl;
    std::cout << "Should print in 0,2,4 seconds" << DEFAULT << std::endl;

    Scheduler* sched = ilrd::Handleton<Scheduler>::GetInstance();
    std::shared_ptr<MyTask> task_ptr = std::make_shared<MyTask>();
    sched->AddTask(task_ptr,  static_cast<std::chrono::milliseconds>(0));
    sched->AddTask(task_ptr,  static_cast<std::chrono::milliseconds>(4000));
    sched->AddTask(task_ptr,  static_cast<std::chrono::milliseconds>(2000));
    sleep(5);
}

void TestSameTimeTasks()
{
    std::cout << BLUE << "TestSameTimeTasks" << std::endl;
    std::cout << "Should print 3 times at the same moment after 3 seconds" << DEFAULT << std::endl;

    Scheduler* sched = ilrd::Handleton<Scheduler>::GetInstance();
    std::shared_ptr<MyTask> task = std::make_shared<MyTask>();
    sched->AddTask(task,  static_cast<std::chrono::milliseconds>(3000));
    sched->AddTask(task,  static_cast<std::chrono::milliseconds>(3000));
    sched->AddTask(task,  static_cast<std::chrono::milliseconds>(3000));
    sleep(4);
}

void TestReentrantTask()
{
    std::cout << BLUE << "TestReentrantTask" << std::endl;
    std::cout << "Should print in 1,3 seconds" << DEFAULT << std::endl;

    Scheduler* sched = ilrd::Handleton<Scheduler>::GetInstance();
    std::shared_ptr<ReentrantTask> task = std::make_shared<ReentrantTask>();
    sched->AddTask(task,  static_cast<std::chrono::milliseconds>(1000));
    sleep(4);
}

int main()
{
    TestSyncScheduler();
    TestAnSyncScheduler();
    TestImmediateScheduler();
    TestSameTimeTasks();
    TestReentrantTask();
    return(0);
}
