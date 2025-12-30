/*name: Dvir Goldhamer

File Name: concrete_utils.hpp

purpose: API for a concrete utils

reviewed by : 

Date: */

#ifndef ILRD_RD168_169_CONCRETE_UTILS_HPP
#define ILRD_RD168_169_CONCRETE_UTILS_HPP

#include <functional>

#include "ilrd_uid.hpp"

namespace ilrd
{

enum STATUS
{
    SUCCESS,
    FAILURE,
    COMPLETE_FAILURE,
    WAITING
};


}//namespace ilrd

namespace std
{

template<>
struct hash<ilrd::UID>
{
    size_t operator()(const ilrd::UID& uid) const noexcept
    {
        size_t h1 = std::hash<uint64_t>()(uid.m_counter);
        size_t h2 = std::hash<pid_t>()(uid.m_pid);
        size_t h3 = std::hash<time_t>()(uid.m_time);
        size_t h4 = std::hash<uint32_t>()(uid.m_ip_addr);

        return (((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1)) ^ (h4 << 1);
    }
};

}//namespace std



#endif //ILRD_RD168_169_CONCRETE_UTILS_HPP