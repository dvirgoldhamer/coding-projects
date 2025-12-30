
/*name: Dvir Goldhamer

File Name: udp_client_server.cpp

purpose: Implementation for a udp clientand server

reviewed by :

Date: */

#include <stdexcept>

#include "handleton.hpp"
#include "raid_manager.hpp"
#include "udp_client_server.hpp"

namespace ilrd

{

int UDPFunctionality::CreateSocket(std::string who_am_i, struct addrinfo** res,std::string ip_address, std::string port)
{
    struct addrinfo hints = {};
    
    InitAddrInfo(&hints, SOCK_DGRAM, 0);
    
   
    if ((GetSocketCandidates(ip_address.c_str(), port.c_str(), &hints, res)) != 0)
    {
        throw std::runtime_error("udp socket creation failed");
    }

    int sockfd;
    CreateUDPSocket(const_cast<char*>(who_am_i.c_str()), &sockfd, res);

    return sockfd;

}

void UDPFunctionality::Send(int sockfd, struct addrinfo* res, char* buffer, uint64_t buffer_length)
{

    if (sendto(sockfd, buffer, buffer_length, 0, res->ai_addr, res->ai_addrlen) < 0) 
    {
        throw std::runtime_error("UDP connect failed");
    }
}

std::shared_ptr<char[]> UDPFunctionality::Receive(int sockfd, uint64_t buffer_length, int flag)
{
    struct sockaddr_storage from_addr;
    socklen_t from_len = sizeof(from_addr);

    auto buffer = std::shared_ptr<char[]>(new char[buffer_length]);
    
    if (recvfrom(sockfd, buffer.get(), buffer_length, flag, reinterpret_cast<struct sockaddr*>(&from_addr), &from_len) < 0)
    {
        throw std::runtime_error("message receive failed");
    }

    return buffer;

}

}//namespace ilrd