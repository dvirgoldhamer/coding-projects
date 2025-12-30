/*name: Dvir Goldhamer

File Name: raid_manager.cpp

purpose: Implementation for a RAID manager class

reviewed by :

Date: */

#include <iostream>

#include "raid_manager.hpp"
#include "key_args.hpp"
#include "minion_proxy.hpp"


namespace ilrd
{

const std::string RAIDManager::MASTER_IP_ADRRESS = "10.10.1.73";
const std::string RAIDManager::MASTER_PORT = "4080";

const std::string RAIDManager::MINION1_IP_ADRRESS = "10.10.1.73";
const std::string RAIDManager::MINION1_PORT = "4090";

const std::string RAIDManager::MINION2_IP_ADRRESS = "10.1.0.146";
const std::string RAIDManager::MINION2_PORT = "4091";

const std::string RAIDManager::MINION3_IP_ADRRESS = "10.10.1.73";
const std::string RAIDManager::MINION3_PORT = "4092";

const size_t RAIDManager::MINION_SIZE = 4 * 1024 * 1024;

RAIDManager::RAIDManager()
{
    m_minions.emplace_back(std::make_shared<MinionProxy>(MINION1_IP_ADRRESS, MINION1_PORT));
    m_minions.emplace_back(std::make_shared<MinionProxy>(MINION2_IP_ADRRESS, MINION2_PORT));
    m_minions.emplace_back(std::make_shared<MinionProxy>(MINION3_IP_ADRRESS, MINION3_PORT));
}

std::pair<UID, UID> RAIDManager::Write(std::shared_ptr<interfaces::IKeyArgs> key_args)
{
    auto write_args = std::static_pointer_cast<WriteArgs>(key_args);

    int global_offset = write_args->GetOffset();
    int minion_index = FindMinionIndex(global_offset);
    int local_offset = FindMinionOffset(global_offset);

    //write to minion
    write_args->SetOffset(local_offset);

    std::pair<UID, UID> commands_uids;

    commands_uids.first = m_minions[minion_index]->Write(write_args);

    //write to back-up minion
    local_offset += MINION_SIZE;
    commands_uids.second = m_minions[(minion_index + 1) % m_minions.size()]->Write(write_args);

    return commands_uids;
}

std::pair<UID, UID> RAIDManager::Read(std::shared_ptr<interfaces::IKeyArgs> key_args)
{
    auto read_args = std::static_pointer_cast<ReadArgs>(key_args);

    int global_offset = read_args->GetOffset();
    int minion_index = FindMinionIndex(global_offset);
    int local_offset = FindMinionOffset(global_offset);
    
    //read from minion
    read_args->SetOffset(local_offset);

    std::pair<UID, UID> commands_uids;

    commands_uids.first = m_minions[minion_index]->Read(read_args);
    
    //read from back-up minion
    local_offset += MINION_SIZE;
    commands_uids.second = m_minions[(minion_index + 1) % m_minions.size()]->Read(read_args);

    return commands_uids;

}

int RAIDManager::FindMinionIndex(int global_offset)
{
    return (global_offset / (MINION_SIZE)) % m_minions.size();
}

int RAIDManager::FindMinionOffset(int global_offset)
{
    return (global_offset % MINION_SIZE);
}

} //namespace ilrd#include "key_args.hpp"