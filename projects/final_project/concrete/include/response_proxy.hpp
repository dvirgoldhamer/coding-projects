/*name: Dvir Goldhamer

File Name: response_proxy.hpp

purpose: API for ResponseProxy

reviewed by : 

Date: */

#ifndef IRLD_RD168_169_RESPONSE_PROXY_HPP
#define IRLD_RD168_169_RESPONSE_PROXY_HPP

#include "framework_interfaces.hpp"
#include "concrete_utils.hpp"
#include "udp_client_server.hpp"
#include "key_args.hpp"

namespace ilrd
{

class ResponseProxy : public interfaces::IInputProxy
{
public:

    explicit ResponseProxy() = default;
    virtual ~ResponseProxy() = default;

    std::shared_ptr<interfaces::IKeyArgs> GetKeyArgs(int fd, ilrd::Mode mode);
  
    ResponseProxy(const ResponseProxy& other) = delete;
    ResponseProxy& operator=(const ResponseProxy& other) = delete;

};

} //namespace ilrd

#endif //IRLD_RD168_169_RESPONSE_PROXY_HPP