/*************************************************************************
* File: ilrd_uid.cpp 
*
* Purpose:
*   
*  creation of UIDs
* 
* Author:
*   Dvir Goldhamer
*
* Review History:
*     
*************************************************************************/

#include <arpa/inet.h>//geditaddrs
#include <unistd.h>// getpid
#include <ifaddrs.h>//geditaddrs
#include <stdexcept>//std runtime_error

#include "ilrd_uid.hpp"

namespace ilrd
{

std::atomic<uint64_t> UID::counter{0};

UID::UID() : m_counter(counter.fetch_add(1) + 1), m_pid(getpid()) 
{       
	if ((m_time = time(NULL)) < 0)
    {
        throw std::runtime_error("uid creation failed");
    }

    if ((m_ip_addr = GetIPAddress()) <= 0)
    {
        throw std::runtime_error("uid creation failed");
    }   
}

int64_t UID::GetBufferLength()
{
    return (sizeof(m_counter) + sizeof(m_pid) + sizeof(m_time) + sizeof(m_ip_addr));
}

char* UID::ToBuffer(char* buffer)
{    
    *(uint64_t*)buffer = m_counter;
    buffer += sizeof(m_counter);
    
    *(pid_t*)buffer = m_pid;
    buffer += sizeof(m_pid);
    
    *(time_t*)buffer = m_time;
    buffer += sizeof(m_time);

    *(uint32_t*)buffer = m_ip_addr;
    buffer += sizeof(m_ip_addr);
    
    return buffer; 
}

char* UID::FromBuffer(char* buffer)
{
    m_counter = *(uint64_t*)buffer;
    buffer += sizeof(m_counter);

    m_pid = *(pid_t*)buffer;
    buffer += sizeof(m_pid);

    m_time = *(time_t*)buffer;
    buffer += sizeof(m_time);

    m_ip_addr = *(uint32_t*)buffer;
    buffer += sizeof(m_ip_addr);

    return buffer; 

}


uint32_t UID::GetIPAddress()
{
    struct ifaddrs* ifap = NULL;
    struct ifaddrs* ifa = NULL;
    struct sockaddr_in* sa = NULL;
    
    getifaddrs(&ifap);

    for (ifa = ifap; NULL != ifa; ifa = ifa->ifa_next)
        {
           if ((ifa->ifa_addr) && (ifa->ifa_addr->sa_family == AF_INET) && (69699 == ifa->ifa_flags))
           {
              sa = (struct sockaddr_in*)ifa->ifa_addr;
              break;
           }
        }

    freeifaddrs(ifap);
    
    return sa->sin_addr.s_addr;  
}

} //namespace ilrd

