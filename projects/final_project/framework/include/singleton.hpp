/*name: Dvir Goldhamer

File Name: singleton.hpp

purpose: API for a singleton

reviewed by Gil: 

Date: 28.07.25*/

#ifndef IRLD_RD168_169__SINGLETON_HPP
#define IRLD_RD168_169__SINGLETON_HPP

#include <mutex>
#include <atomic>

namespace ilrd
{

template <typename T>
class SingletonDestroyer;

template <typename T>
class  Singleton
{
public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    ~Singleton() = default;
    
    static T* GetInstance();

private:
    Singleton() {}
    
    static std::mutex s_mtx;
    static std::atomic<T*> s_instancePtr;
    static const uintptr_t DEAD_BEEF = 0xdeadbeef;

    friend class SingletonDestroyer<T>;
};

template <typename T>
class SingletonDestroyer
{
    public:
    SingletonDestroyer(const SingletonDestroyer&) = delete;
    SingletonDestroyer& operator=(const SingletonDestroyer&) = delete;
    
    ~SingletonDestroyer();
    SingletonDestroyer() = default;    
};

template <typename T>
SingletonDestroyer<T>::~SingletonDestroyer()
{
    delete Singleton<T>::s_instancePtr;

    Singleton<T>::s_instancePtr.store(reinterpret_cast<T*>(Singleton<T>::DEAD_BEEF), std::memory_order_release);
}


template <typename T>
std::mutex Singleton<T>::s_mtx;

template <typename T>
std::atomic<T*> Singleton<T>::s_instancePtr = nullptr;

template <typename T>
T* Singleton<T>::GetInstance()
{
    //assign ptr to tmp var atomically (reads from memory to register)
    //acquire guarantees visibilty to all writes before acquire
    T* tmp = s_instancePtr.load(std::memory_order_acquire);

    if (tmp == nullptr)
    {        
        std::unique_lock<std::mutex> lock(s_mtx);
        
        //assign ptr to tmp var atomically (reads from memory to register)
        tmp = s_instancePtr.load(std::memory_order_relaxed);

        if (tmp == nullptr)
        {
            static SingletonDestroyer<T> destroyer;
            tmp = new T();

            //writes tmp atomically into instance ptr (register to memory)
            //release make all previos writes visible to other threads - 
            //ensures threads see a fully constructed object
            s_instancePtr.store(tmp, std::memory_order_release);

           // std::atexit([] { delete Singleton<T>::s_instancePtr.load(std::memory_order_acquire);});
        }
    }

    return tmp;
}

} //namespace ilrd

#endif //IRLD_RD168_169__SINGLETON_HPP