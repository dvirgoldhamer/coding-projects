/*name: Dvir Goldhamer

File Name: thread_pool.hpp

purpose: API for a thread pool

reviewed by : Yonatan Feiner 

Date: 23.07.25*/

#ifndef IRLD_RD168_169_THREAD_POOL_HPP
#define IRLD_RD168_169_THREAD_POOL_HPP

#include <functional> //std::function
#include <memory> //std::shared_ptr
#include <semaphore>
#include <thread>

#include "waitable_queue.hpp"

namespace ilrd
{
const int NUM_OF_CORES = std::thread::hardware_concurrency();
const int MAX_THREADS = 50; //max threads to use, may affect performance

class ThreadPool
{
public:
    ThreadPool(size_t num_threads = NUM_OF_CORES/2); //non-explicit on purpose
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool& other) = delete;
    
    ~ThreadPool();
    
    class ITPTask
    {
    public:
        virtual void Run() = 0;
        virtual ~ITPTask() = default;
    };
    
    class FunctionTask : public ITPTask
    {
    public:
        explicit FunctionTask(std::function<void()> func);
        ~FunctionTask() = default;

        void Run();
        
    private:
        std::function<void()> m_func;
    }; 
    
    template <typename RET_VAL>
    class FutureTask : public ITPTask
    {
    public:
        FutureTask(std::function<RET_VAL()> fnc);
        ~FutureTask() = default;

        RET_VAL GetResult();
        
        void Run();// may throw any exception from func
        
    private:
        RET_VAL m_res;
        std::function<RET_VAL()> m_futureFunc;
        std::binary_semaphore m_futureSem;
    };
        
    enum class Priority
    {
        PRIVATE_LOWEST, //do not use, internal usage
        LOW,
        MID,
        HIGH,
        PRIVATE_HIGHEST //do not use, internal usage
    };

    // may throw std::system_error, std::invalid_argument if task is null
    void AddTask(std::shared_ptr<ITPTask> task, Priority prior = Priority::HIGH);
    
    // may throw std::system_error, blocks until all threads are paused
    void Pause();

    // may throw std::system_error
    void Resume();
    void SetNumberOfThreads(size_t num);//blocking (if reduces)

private:

    struct TaskItem
    {
        std::shared_ptr<ITPTask> m_task; 
        Priority m_prior;
        bool operator<(const TaskItem& item) const;
    };

    std::unordered_map<std::thread::id, std::thread> m_threads;
    WaitableQueue<TaskItem, PriorityQueue<TaskItem>> m_pq;
    size_t m_numThreads;

    void WorkerThread();

    std::thread::id PoisonTask();
    void PauseTask();
    void AddThreads(size_t howMany);

    std::counting_semaphore<> m_workSem;
    std::mutex m_mtx;
    std::condition_variable m_cond;

    bool m_PauseFlag;
    static thread_local bool s_localIsRunning; 
};
/**************************************************************************/
template <typename RET_VAL>
ThreadPool::FutureTask<RET_VAL>::FutureTask(std::function<RET_VAL()> fnc) :
m_futureFunc(fnc), m_futureSem(0)
{
}

template <typename RET_VAL>
RET_VAL ThreadPool::FutureTask<RET_VAL>::GetResult()
{
    m_futureSem.acquire();
    return m_res;
}

template <typename RET_VAL>
void ThreadPool::FutureTask<RET_VAL>::Run()
{
    m_res = m_futureFunc();
    m_futureSem.release();
}

} //namespace ilrd

#endif //IRLD_RD168_169_THREAD_POOL_HPP