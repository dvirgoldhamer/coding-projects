/*name: Dvir Goldhamer

File Name: udp_client_server.hpp

purpose: API for a UDP communication

reviewed by : 

Date: */

#ifndef IRLD_RD168_169_UDP_CLIENT_SERVER_HPP
#define IRLD_RD168_169_UDP_CLIENT_SERVER_HPP

#include "network_util.hpp"
#include "raid_manager.hpp"

namespace ilrd
{

class UDPFunctionality
{
public:

    static int CreateSocket( std::string who_am_i, struct addrinfo** res, std::string ip_address, std::string port);

    static void Send(int sockfd, struct addrinfo* res, char* buffer, uint64_t buffer_length);

    static std::shared_ptr<char[]> Receive(int sockfd, uint64_t buffer_length, int flag);

};

} //namespace ilrd

#endif //IRLD_RD168_169_UDP_CLIENT_SERVER_HPP

