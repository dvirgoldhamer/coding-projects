/*name: Dvir Goldhamer

File Name: thread_pool.hpp

purpose: Implementation for a thread pool class

reviewed by Yonatan Feiner: 

Date: 23.07.25*/

#include <iostream>

#include "thread_pool.hpp"
#include "handleton.hpp"

namespace ilrd
{

thread_local bool ThreadPool::s_localIsRunning = true;

/**************************************************************************/
ThreadPool::ThreadPool(size_t numThreads) :m_numThreads(0), m_workSem(0), m_PauseFlag(false) 
{
    AddThreads(numThreads);
}

ThreadPool::~ThreadPool()
{

    std::shared_ptr<ITPTask> poisonTask = std::make_shared<FutureTask<std::thread::id>>( [this] { return PoisonTask(); });
    try
    {
        for (size_t i = 0; i < m_numThreads; ++i)
        {
            AddTask(poisonTask, Priority::PRIVATE_LOWEST);
        }

        for (auto& pair : m_threads)
        {
            pair.second.join();
        }    
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void ThreadPool::AddThreads(size_t howMany)
{
    for (size_t i = 0; i < howMany; ++i)
    {
        std::thread thr( [this] { WorkerThread(); });
        m_threads.emplace(thr.get_id(), std::move(thr));
        ++m_numThreads;
    }
}
/**************************************************************************/
ThreadPool::FunctionTask::FunctionTask(std::function<void()> func) : 
m_func(func) 
{
}

void ThreadPool::FunctionTask::Run()
{
    m_func();
}

void ThreadPool::AddTask(std::shared_ptr<ITPTask> task,Priority prior)
{
    m_pq.Push(TaskItem(task, prior));
}

bool ThreadPool::TaskItem::operator<(const TaskItem& item) const
{
    return static_cast<int>(m_prior) < static_cast<int>(item.m_prior);
}

void ThreadPool::SetNumberOfThreads(size_t requestedThreadNum)
{
    
    if (requestedThreadNum < m_numThreads) //need to reduce threads
    {
        std::vector<std::shared_ptr<FutureTask<std::thread::id>>> vct;
        
        for (size_t i = 0; i < m_numThreads - requestedThreadNum; ++i)
        {
            auto poisonTask = std::make_shared<FutureTask<std::thread::id>>( [this] { return PoisonTask(); });

            vct.push_back(poisonTask); 
            
            AddTask(poisonTask, Priority::PRIVATE_HIGHEST);
        }
        
        for (auto& task : vct)
        {
            auto thrId = task->GetResult();
            
            m_threads[thrId].join();
            
            m_threads.erase(thrId);
            
            --m_numThreads;
        }
    }
    else
    {
        AddThreads(requestedThreadNum - m_numThreads);
    }
}

void ThreadPool::Pause()
{         

    m_PauseFlag = true;

    std::shared_ptr<ITPTask> pauseTask = std::make_shared<FunctionTask>( [this]() { PauseTask(); });
    for (size_t i = 0; i < m_numThreads; ++i)
    {
        AddTask(pauseTask, Priority::PRIVATE_HIGHEST);
    }
    
    for (size_t i = 0; i < m_numThreads; ++i)
    {
        m_workSem.acquire();
    }
}

void ThreadPool::Resume()
{
    m_PauseFlag = false;

    std::unique_lock<std::mutex> lock(m_mtx);
    m_cond.notify_all();
}

void ThreadPool::WorkerThread()
{
    while(s_localIsRunning)
    {
        TaskItem task;
        m_pq.Pop(task);
        task.m_task->Run();
    }
}
/**************************************************************************/
std::thread::id ThreadPool::PoisonTask()
{
    s_localIsRunning = false;

    return std::this_thread::get_id();
}

void ThreadPool::PauseTask()
{
    m_workSem.release();
    std::unique_lock<std::mutex> lock(m_mtx);
    m_cond.wait(lock, [this]() { return m_PauseFlag = false; });
} 
} //namespace ilrds