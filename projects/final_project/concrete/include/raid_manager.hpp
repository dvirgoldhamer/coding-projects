/*name: Dvir Goldhamer

File Name: raid_manager.hpp

purpose: API for a raid_manager

reviewed by : 

Date: */

#ifndef IRLD_RD168_169_RAID_MANAGER_HPP
#define IRLD_RD168_169_RAID_MANAGER_HPP

#include <memory>
#include <vector>
#include <utility> //std::pair


#include "handleton.hpp"
#include "iminion_proxy.hpp"
#include "ilrd_uid.hpp"

namespace ilrd
{

class RAIDManager
{

public:
    RAIDManager(const RAIDManager&) = delete;
    RAIDManager& operator=(const RAIDManager&) = delete;

    std::pair<UID, UID> Read(std::shared_ptr<interfaces::IKeyArgs> key_args);
    std::pair<UID, UID> Write(std::shared_ptr<interfaces::IKeyArgs> key_args);

    static const std::string MASTER_IP_ADRRESS;
    static const std::string MASTER_PORT;

    static const std::string MINION1_IP_ADRRESS;
    static const std::string MINION1_PORT;

    static const std::string MINION2_IP_ADRRESS;
    static const std::string MINION2_PORT;

    static const std::string MINION3_IP_ADRRESS;
    static const std::string MINION3_PORT;
    
    static const size_t MINION_SIZE;
    
private:

    std::vector<std::shared_ptr<IMinionProxy>> m_minions;

    RAIDManager();
    ~RAIDManager() = default;

    int FindMinionOffset(int global_offset);
    int FindMinionIndex(int global_offset);

    friend class Handleton<RAIDManager>;

};

} //namespace ilrd

#endif //IRLD_RD168_169_RAID_MANAGER_HPP