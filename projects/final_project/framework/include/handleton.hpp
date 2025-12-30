/*name: Dvir Goldhamer

File Name: Handleton.hpp

purpose: API for a Handleton

reviewed by Yonatan Feiner: 

Date: 29.07.25*/

#ifndef IRLD_RD168_169__HANDLETON_HPP
#define IRLD_RD168_169__HANDLETON_HPP

#ifdef DLL_IMPLEMENTOR

#include <iostream>
#include <mutex>
#include <atomic>

namespace ilrd
{

template <typename T>
class Handleton
{
public:
    Handleton(const Handleton&) = delete;
    Handleton& operator=(const Handleton&) = delete;
    ~Handleton() = default;
    Handleton() = delete;
    
    static T* GetInstance();

private:
    
    static std::mutex s_mtx;
    static std::atomic<T*> s_instancePtr;
    static const uintptr_t DEAD_BEEF = 0xdeadbeef;

};

template <typename T>
std::mutex Handleton<T>::s_mtx;

template <typename T>
std::atomic<T*> Handleton<T>::s_instancePtr = nullptr;

template <typename T>
T* Handleton<T>::GetInstance()
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
            tmp = new T;

            //writes tmp atomically into instance ptr (register to memory)
            //release make all previos writes visible to other threads - 
            //ensures threads see a fully constructed object
            s_instancePtr.store(tmp, std::memory_order_release);

            std::atexit([] { 
            delete s_instancePtr.load(std::memory_order_acquire);
            s_instancePtr.store(reinterpret_cast<T*>(DEAD_BEEF), std::memory_order_release);});
        }
    }

    return tmp;
}
} // namespace ilrd

//forcing instantiation before usage (to avoid linking problem)

#define INSTANTIATE_HANDLETON(T) template class ilrd::Handleton<T>;

#else
namespace ilrd
{

template <typename T>
class Handleton
{
public:
    static T* GetInstance();
};


} //namespace ilrd

#endif 
#endif //IRLD_RD168_169__HANDLETON_HPP
