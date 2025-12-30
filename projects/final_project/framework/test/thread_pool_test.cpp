#include <iostream>
#include <memory>
#include <atomic>
#include <syncstream>

#include "thread_pool.hpp"

#define BLUE ("\x1B[34m")
#define DEFAULT ("\033[0m")

int FutureTaskThreadFunction();
void TestFutureTask();

void FunctionTaskThreadFunction();
void TestFunctionTask();

void PauseThreadFunction();
void TestPauseResume();

void SetNumberThreadFunction();
void TestSetNumberThreads();

void TestPriorities();
void PriorityTaskFunction(const std::string& name);

int main()
{
    TestFunctionTask();
    TestFutureTask();
     TestPauseResume();
    TestSetNumberThreads();
     TestPriorities();
    
    return (0);
}

void PriorityTaskFunction(const std::string& name)
{
    std::osyncstream(std::cout) << "Executing " << name << " on thread "
                                << std::this_thread::get_id() << std::endl;
    sleep(1);
}

void TestPriorities()
{
    std::osyncstream(std::cout) << BLUE << "Test Priorities:" << DEFAULT << std::endl;

    ilrd::ThreadPool pool(2);
    using Task = ilrd::ThreadPool::FunctionTask;

    pool.Pause();

    // Low priority tasks
    for (int i = 0; i < 3; ++i)
    {
        std::string name = "LOW task " + std::to_string(i + 1);
        auto task = std::make_shared<Task>([name]() { PriorityTaskFunction(name); });
        pool.AddTask(task, ilrd::ThreadPool::Priority::LOW);
    }

    // High priority tasks
    for (int i = 0; i < 3; ++i)
    {
        std::string name = "HIGH task " + std::to_string(i + 1);
        auto task = std::make_shared<Task>([name]() { PriorityTaskFunction(name); });
        pool.AddTask(task, ilrd::ThreadPool::Priority::HIGH);
    }

    // Medium priority tasks
    for (int i = 0; i < 3; ++i)
    {
        std::string name = "MEDIUM task " + std::to_string(i + 1);
        auto task = std::make_shared<Task>([name]() { PriorityTaskFunction(name); });
        pool.AddTask(task, ilrd::ThreadPool::Priority::MID);
    }

    pool.Resume();

    std::osyncstream(std::cout) << "All tasks submitted.\nExpected: HIGH first, then MEDIUM, then LOW.\n" << std::endl;

    sleep(10); // Wait for all tasks to complete
}

void SetNumberThreadFunction()
{
    std::osyncstream(std::cout) << "Thread ID: " << std::this_thread::get_id() << " executing\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void TestSetNumberThreads()
{
    std::osyncstream(std::cout) << BLUE << "Test Set Number Of Threads:" << DEFAULT << std::endl;

    ilrd::ThreadPool pool(5);
    auto task_ptr = std::make_shared<ilrd::ThreadPool::FunctionTask>(SetNumberThreadFunction);

    std::osyncstream(std::cout) << "Adding 10 tasks with 5 threads..." << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        pool.AddTask(task_ptr, ilrd::ThreadPool::Priority::LOW);
    }

    sleep(1);
    std::osyncstream(std::cout) << "\nReducing to 3 threads..." << std::endl;
    pool.SetNumberOfThreads(3);

    sleep(2);
    std::osyncstream(std::cout) << "\nReducing to 2 threads..." << std::endl;
    pool.SetNumberOfThreads(2);

    sleep(2);
}

void PauseThreadFunction()
{
    std::osyncstream(std::cout) << "thread working for 5 seconds" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

void TestPauseResume()
{
    std::osyncstream(std::cout) << BLUE << "Test Pause Resume:" << DEFAULT << std::endl;

    ilrd::ThreadPool pool(5);

    auto task_ptr = std::make_shared<ilrd::ThreadPool::FunctionTask>(PauseThreadFunction);

    for(int i = 0 ; i < 10 ; ++i)
    {
        pool.AddTask(task_ptr, ilrd::ThreadPool::Priority::LOW);
    }
    sleep(1);
    pool.Pause();

    std::osyncstream(std::cout) << "pause - in 2 seconds threads will resume" << std::endl;
    sleep(2);

    pool.Resume();
    sleep(1);/* problem ?*/
}

void FunctionTaskThreadFunction()
{
    static std::atomic<int> count = 0;
    ++count;
    std::osyncstream(std::cout) << count << std::endl;
}

void TestFunctionTask()
{
    std::osyncstream(std::cout) << BLUE << "Test Function Task:" << DEFAULT << std::endl;
    std::osyncstream(std::cout) << BLUE << "Should print 1-5" << DEFAULT << std::endl;

    ilrd::ThreadPool pool(5);

    auto task_ptr = std::make_shared<ilrd::ThreadPool::FunctionTask>(FunctionTaskThreadFunction);

    for(int i = 0 ; i < 5 ; ++i)
    {
        pool.AddTask(task_ptr, ilrd::ThreadPool::Priority::LOW);
        sleep(1);
    }
}

int FutureTaskThreadFunction()
{
    static std::atomic<int> count = 0;
    ++count;
    return (count);
}

void TestFutureTask()
{
    std::osyncstream(std::cout) << BLUE << "Test Future Task:" << DEFAULT << std::endl;
    std::osyncstream(std::cout) << BLUE << "Should print 1-5" << DEFAULT << std::endl;

    ilrd::ThreadPool pool(5);
    auto task_ptr = std::make_shared<ilrd::ThreadPool::FutureTask<int>>(FutureTaskThreadFunction);

    for(int i = 0 ; i < 5 ; ++i)
    {
        pool.AddTask(task_ptr, ilrd::ThreadPool::Priority::LOW);
        std::osyncstream(std::cout) << task_ptr->GetResult() << std::endl;
        sleep(1);
    }
}