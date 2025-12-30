/*name: Dvir Goldhamer

File Name: waitable_queue.hpp

purpose: API for waitable queue class

reviewed by Sharona Amran: 

Date: 21.07.25*/

#ifndef IRLD_RD168_169_WAITABLE_QUEUE_HPP
#define IRLD_RD168_169_WAITABLE_QUEUE_HPP

#include <queue>
#include <chrono>
#include <mutex>
#include <condition_variable>

namespace ilrd
{
template <typename T,typename Q = std::queue<T>>
class WaitableQueue
{
public:

    explicit WaitableQueue() = default;
    WaitableQueue(const WaitableQueue& other) = delete;
    WaitableQueue& operator=(const WaitableQueue& other) = delete;
    ~WaitableQueue() = default;

    void Push (const T& data); //can throw system_error
    void Pop(T& outparam);//can throw system_error
    bool Pop(T& outparam, size_t timeout);//can throw system_error

    bool Empty() const; //can throw system_error
    //if there is a context switch after the checking of the queue, than another thread changes the queue, and there is a switch back to the first thread - the returning state would be incorrect


private:
    Q m_queue;
    mutable std::timed_mutex m_mtx;
    std::condition_variable_any m_cond;
};

template <typename T>
class PriorityQueue: private std::priority_queue<T>
{
public:
    using std::priority_queue<T>::push;
    using std::priority_queue<T>::pop;
    using std::priority_queue<T>::empty;
    const T& front();
};

template <typename T,typename Q>
void WaitableQueue<T, Q>::Push(const T& data)
{
    {
        std::unique_lock<std::timed_mutex> lock(m_mtx);
        m_queue.push(data);
    }

    m_cond.notify_one();
}

template <typename T,typename Q>
void WaitableQueue<T, Q>::Pop(T& outparam)
{
    std::unique_lock<std::timed_mutex> lock(m_mtx);
    m_cond.wait(lock, [this]() { return !m_queue.empty(); });
    outparam = m_queue.front();
    m_queue.pop();
}

template <typename T,typename Q>
bool WaitableQueue<T, Q>::Pop(T& outparam, size_t timeout)
{
    std::unique_lock<std::timed_mutex> lock(m_mtx, std::defer_lock);
    //doesnt lock yet

    auto timeout_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeout); //if there is a context switch to more than timeout - try locks will fail immediately.
    
    if(!lock.try_lock_until(timeout_time))
    {
        return (false);
    }
    //now lock is already acquired

    if(!m_cond.wait_until(lock, timeout_time, [this] 
      { return !m_queue.empty();}))
    {
        return (false);
    }
    //if true is returned(is_empty) - lock is kept. can work on queue.
    //if false - lock is unlocked, wait until notify or timeout

    outparam = m_queue.front();
    m_queue.pop();

    return true;
}

template <typename T,typename Q>
bool WaitableQueue<T, Q>::Empty() const
{
    std::unique_lock<std::timed_mutex> lock(m_mtx);
    return m_queue.empty();
}

template <typename T>
const T& PriorityQueue<T>::front() 
{
    return std::priority_queue<T>::top();
}

} //namespace ilrd

#endif //IRLD_RD168_169_WAITABLE_QUEUE_HPP