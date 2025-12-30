/*name: Dvir Goldhamer

File Name: master_proxy.hpp

purpose: API for MasterProxy

reviewed by : 

Date: */

#ifndef IRLD_RD168_169_MASTER_PROXY_HPP
#define IRLD_RD168_169_MASTER_PROXY_HPP

#include <fstream>

#include "framework_interfaces.hpp"
#include "udp_client_server.hpp"
#include "concrete_utils.hpp"
#include "ilrd_uid.hpp"

namespace ilrd
{

class MasterProxy : public interfaces::IInputProxy
{
public:

    explicit MasterProxy();
    virtual ~MasterProxy() = default;

    static void SendReadResponse(std::shared_ptr<char[]> buffer, uint64_t buffer_length, STATUS read_status, UID uid);

    static void SendWriteResponse(STATUS write_status, UID uid);

    std::shared_ptr<interfaces::IKeyArgs> GetKeyArgs(int fd, Mode mode) override;

    MasterProxy(const MasterProxy& other) = delete;
    MasterProxy& operator=(const MasterProxy& other) = delete;

private:
    inline static int m_sockfd = -1;
    inline static struct addrinfo* m_res = {};
};

} //namespace ilrd

#endif //IRLD_RD168_169_MASTER_PROXY_HPP