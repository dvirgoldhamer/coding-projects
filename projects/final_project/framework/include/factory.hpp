/*name: Dvir Goldhamer

File Name: factory.hpp

purpose: API for a factory

reviewed by : Shani Zuniga

Date: 27.07.25*/

#include <functional> //std::function
#include <memory> //shared_ptr
#include <exception>
#include <string>

#include "handleton.hpp"

#ifndef IRLD_RD168_169_FACTORY_HPP
#define IRLD_RD168_169_FACTORY_HPP

namespace ilrd
{
    
namespace factory_details
{
    
class InvalidKey;

template<typename KEY, typename BASE, typename...ARGS>
class Factory
{
    public:
    Factory() = default;
    ~Factory() = default;
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;
    
    void Register(const KEY& key, std::function<std::shared_ptr<BASE>(ARGS...)> creatorFnc); 
    //may throw
    
    std::shared_ptr<BASE> CreateObject(const KEY& key, ARGS... args) const;
    //may throw
    
    private:
    std::unordered_map<KEY, std::function<std::shared_ptr<BASE>(ARGS...)>> m_creators;
    
};

template<typename KEY, typename BASE, typename...ARGS>
void Factory<KEY, BASE, ARGS...>::Register(const KEY& key, std::function<std::shared_ptr<BASE>(ARGS... args)> creatorFnc) 
{
    m_creators[key] = creatorFnc;
}

template<typename KEY, typename BASE, typename...ARGS>
std::shared_ptr<BASE> Factory<KEY, BASE, ARGS...>::CreateObject(const KEY& key, ARGS... args) const 
{
    auto it = m_creators.find(key); 
    if (it == m_creators.end())
    {
        throw InvalidKey("invalid key was provided");
    }
    
    return (it->second(args...));
}

/*********************************************************************/

class InvalidKey : public std::invalid_argument
{
public: 
    inline explicit InvalidKey(const std::string& message);
    ~InvalidKey() = default;
    InvalidKey(const InvalidKey& other) = default;

    inline const char* what() const noexcept;

private:
    std::string m_message;
};

InvalidKey::InvalidKey(const std::string& message) : std::invalid_argument(message), m_message(message)
{
}

const char* InvalidKey::what() const noexcept
{
    return m_message.c_str();
}

} //namespace factory_details
} //namespace ilrd

#endif //IRLD_RD168_169_FACTORY_HPP