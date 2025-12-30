/*name: Dvir Goldhamer

File Name: framework.hpp

purpose: Interfaces for a framework prototype

reviewed by : 

Date: */

#ifndef ILRD_RD168_169_FRAMEWORK_INTERFACES_HPP
#define ILRD_RD168_169_FRAMEWORK_INTERFACES_HPP

#include <memory> //std::shared_ptr
#include <functional>
#include <chrono>

#include "ilrd_uid.hpp"

namespace ilrd
{
    
enum Mode
{
    READ,
    WRITE
};

namespace interfaces
{

class IKeyArgs
{
public:
    virtual int GetKey() = 0;
};

class IInputProxy
{
public:
    virtual std::shared_ptr<IKeyArgs> GetKeyArgs(int fd, ilrd::Mode mode) = 0;
};

class ICommand
{
public:
    virtual std::pair<std::function<bool()>,std::chrono::milliseconds> Execute(std::shared_ptr<IKeyArgs> key_args) = 0;

    static bool AsyncFunc(std::pair<UID,UID> commands_uids);
};

} //namespace interfaces
}//namespace ilrd

#endif //IRLD_RD168_169_FRAMEWORK_INTERFACES_HPP