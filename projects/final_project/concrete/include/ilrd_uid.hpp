/*************************************************************************
* File: ilrd_uid.hpp
*
* Purpose:
*   
*  API for creation of UIDs.
* 
* Author:
*   Dvir Goldhamer
*
* Review History:
*     
*
* Date:
*   
*************************************************************************/
#ifndef IRLD_RD168_169_ILRD_UID_HPP
#define IRLD_RD168_169_ILRD_UID_HPP

#include <cstdint>/*for int32_t, int64_t*/
#include <time.h> /*for time()*/
#include <sys/types.h>/*for pid_t*/
#include <atomic>


namespace ilrd
{

class UID
{
public:

    UID(); //throws runtime_error
    ~UID() = default;
    UID(const UID&) = default;
    UID& operator=(const UID&) = default;

    char* ToBuffer(char* buffer);
    char* FromBuffer(char* buffer);
    int64_t GetBufferLength();

    uint64_t m_counter;
    pid_t m_pid;
    time_t m_time;
    uint32_t m_ip_addr;

    bool operator==(const UID& other) const
    {
        return (m_counter == other.m_counter && m_pid == other.m_pid &&
               m_time == other.m_time && m_ip_addr  == other.m_ip_addr);
    }

private:

    static uint32_t GetIPAddress();
    static std::atomic<uint64_t> counter;

};

} //namespace ilrd
#endif //IRLD_RD168_169_ILRD_UID_HPP