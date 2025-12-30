/*name: Dvir Goldhamer

File Name: iminion_proxy.hpp

purpose: API for IMinionProxy

reviewed by : 

Date: */

#ifndef IRLD_RD168_169_IMINION_PROXY_HPP
#define IRLD_RD168_169_IMINION_PROXY_HPP

#include <cstdint> //uint16_t
#include <memory> //shared_ptr

#include "framework_interfaces.hpp"
#include "handleton.hpp"
#include "ilrd_uid.hpp"

namespace ilrd
{

class IMinionProxy
{
public:

virtual UID Read(std::shared_ptr<interfaces::IKeyArgs> key_args) = 0;
virtual UID Write(std::shared_ptr<interfaces::IKeyArgs> key_args) = 0;

virtual ~IMinionProxy() = default;

private:
};

} //namespace ilrd

#endif //IRLD_RD168_169_IMINION_PROXY_HPP