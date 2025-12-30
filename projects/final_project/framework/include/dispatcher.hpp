/*name: Dvir Goldhamer

File Name: dispatcher.hpp

purpose: API for a Dispatcher

reviewed by Ofir Hasson

Date: 03.08.25*/

#ifndef IRLD_RD168_169__DISPATCHER_HPP
#define IRLD_RD168_169__DISPATCHER_HPP

#include <unordered_set>
#include <iostream> //cerr

namespace ilrd
{

template <typename EVENT> 
class Dispatcher;

template <typename EVENT> 
class CallbackBase
{
public:

    explicit CallbackBase() = default;
    CallbackBase(const CallbackBase&) = delete;
    CallbackBase& operator=(const CallbackBase&) = delete;
    virtual ~CallbackBase() noexcept;

    virtual void Notify(const EVENT& event) = 0; //may throw
    virtual void NotifyDeath() = 0; //may throw

private:
    friend class Dispatcher<EVENT>;

    Dispatcher<EVENT>* m_subject = nullptr;
}; //class CallbackBase

template <typename EVENT,typename OBSERVER>
class Callback : public CallbackBase<EVENT>
{
public:
    using NotifyFuncPtr = void (OBSERVER::*)(const EVENT&);
    using DeathFuncPtr = void (OBSERVER::*)();

    explicit Callback(OBSERVER& object ,NotifyFuncPtr notify_func, DeathFuncPtr death_func = nullptr);
    Callback(const Callback&) = delete;
    Callback& operator=(const Callback&) = delete;
    ~Callback() = default;

    
private:
    
    OBSERVER& m_object;
    NotifyFuncPtr m_notify_func;
    DeathFuncPtr m_death_func;

    virtual void Notify(const EVENT& event) override; //throws what callback notify member function throws
    virtual void NotifyDeath() override; //throws what callback death member function throws
}; //class Callback

template<typename EVENT>
class Dispatcher
{   
public:
    explicit Dispatcher();
    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;
    ~Dispatcher() noexcept;

    void Subscribe(CallbackBase<EVENT>* cb_ptr);
    void Unsubscribe(CallbackBase<EVENT>* cb_ptr);
    void Notify(const EVENT& event); //throws what callback->notify throws

private:
    std::unordered_set<CallbackBase<EVENT>*> m_subscribers;
    
}; //class Dispatcher
/*****************************CallbackBase************************************/
template <typename EVENT> 
CallbackBase<EVENT>::~CallbackBase() noexcept
{
    if (m_subject)
    {
        m_subject->Unsubscribe(this);
    }   
}
/*********************************Callback*************************************/
template <typename EVENT, typename OBSERVER> 
Callback<EVENT, OBSERVER>::Callback(OBSERVER& object,NotifyFuncPtr notify_func,DeathFuncPtr death_func) : CallbackBase<EVENT>(), m_object(object), m_notify_func(notify_func), m_death_func(death_func)
{
}

template <typename EVENT, typename OBSERVER> 
void Callback<EVENT, OBSERVER>::Notify(const EVENT& event)
{
    (m_object.*m_notify_func)(event);
}

template <typename EVENT, typename OBSERVER> 
void Callback<EVENT, OBSERVER>::NotifyDeath()
{
    if (m_death_func)
    {
        (m_object.*m_death_func)();
    }
}

/********************************Dispatcher************************************/

template <typename EVENT>
Dispatcher<EVENT>::Dispatcher() : m_subscribers()
{
}

template<typename EVENT>
void Dispatcher<EVENT>::Subscribe(CallbackBase<EVENT>* cb_ptr)
{
    m_subscribers.emplace(cb_ptr);
    cb_ptr->m_subject = this;
}

template<typename EVENT>
void Dispatcher<EVENT>::Unsubscribe(CallbackBase<EVENT>* cb_ptr)
{
    m_subscribers.erase(cb_ptr);
    cb_ptr->m_subject = nullptr;

}

template<typename EVENT>
void  Dispatcher<EVENT>::Notify(const EVENT& event)
{   
    std::unordered_set<CallbackBase<EVENT>*> copies(m_subscribers);

    for (CallbackBase<EVENT>* subscriber : copies)
    {
        subscriber->Notify(event);
    }
}

template<typename EVENT>
Dispatcher<EVENT>::~Dispatcher() noexcept
{
    try
    {
        for (auto it : m_subscribers)
        {
           it->NotifyDeath();
           it->m_subject = nullptr;  
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

} //namespace ilrd

#endif //IRLD_RD168_169__DISPATCHER_HPP
