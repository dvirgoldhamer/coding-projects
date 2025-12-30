/*name: Dvir Goldhamer

File Name: minion_proxy.hpp

purpose: API for MinionProxy

reviewed by : 

Date: */

#ifndef IRLD_RD168_169_MINION_PROXY_HPP
#define IRLD_RD168_169_MINION_PROXY_HPP

#include "iminion_proxy.hpp"
#include "udp_client_server.hpp"

namespace ilrd
{

class MinionProxy : public IMinionProxy
{
public:

    explicit MinionProxy(const std::string ip_address, std::string port);
    virtual ~MinionProxy() = default;

    virtual UID Read(std::shared_ptr<interfaces::IKeyArgs> key_args);
    virtual UID Write(std::shared_ptr<interfaces::IKeyArgs> key_args);

    MinionProxy(const MinionProxy& other) = delete;
    MinionProxy& operator=(const MinionProxy& other) = delete;

private:
    int m_sockfd;
    struct addrinfo* m_res;
};

} //namespace ilrd

#endif //IRLD_RD168_169_MINION_PROXY_HPP