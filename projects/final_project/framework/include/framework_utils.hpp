/*name: Dvir Goldhamer

File Name: framework.hpp

purpose: API for a framework utils

reviewed by : 

Date: */

#ifndef ILRD_RD168_169_FRAMEWORK_UTILS_HPP
#define ILRD_RD168_169_FRAMEWORK_UTILS_HPP

#include <unordered_map>
#include <functional>

#include "framework_interfaces.hpp"


namespace ilrd
{

struct FdModeWrapper
{
    int m_fd;
    Mode m_mode;

    bool operator==(const FdModeWrapper& other) const
    {
        return m_fd == other.m_fd && m_mode == other.m_mode;
    }
    
};

struct FdModeHash
{
    std::size_t operator()(const FdModeWrapper& key) const noexcept
    {
        std::size_t h1 = std::hash<int>{}(key.m_fd);
        std::size_t h2 = std::hash<Mode>{}(key.m_mode);
        return h1 ^ (h2 << 1);
    }
};

}//namespace ilrd

#endif //ILRD_RD168_169_FRAMEWORK_UTILS_HPP