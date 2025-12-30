/*name: Dvir Goldhamer

File Name: response_proxy.cpp

purpose: Implementation for a repsponse proxy class

reviewed by :

Date: */

#include <iostream>

#include "response_manager.hpp"
#include "nbd_proxy.hpp"
#include "handleton.hpp"
#include "concrete_utils.hpp"


namespace ilrd
{

void ResponseManager::AddCommand(unsigned long long nbdID, UID uid)
{
    m_mtx.lock();
    m_commands[uid] = {nbdID, WAITING, nullptr};
    m_singleCommands.insert(nbdID);
    m_mtx.unlock();
}

void ResponseManager::UpdateStatus(UID uid, STATUS status, std::shared_ptr<char[]> buffer, uint64_t buffer_size)
{
    m_mtx.lock();
    m_commands[uid].status = status;
    
    auto nbdID = m_commands[uid].nbd_id;

    //if key exist in the single command map - a response wasnt set yet to the user. response will be sent and the command will be deleted from the single-command map

    if ((status == SUCCESS || status == COMPLETE_FAILURE) && (m_singleCommands.count(nbdID) > 0))
    {
        Handleton<NBDHandler>::GetInstance()->SendResponse(nbdID, status, buffer.get(), buffer_size);

        m_singleCommands.erase(nbdID);
    }
    
    m_mtx.unlock();
}

STATUS ResponseManager::GetCommandStatus(UID uid)
{
    m_mtx.lock();
    auto it = m_commands.find(uid);
    m_mtx.unlock();

    return it->second.status;
}

void ResponseManager::DeleteCommand(UID uid)
{    
    m_mtx.lock();
    m_commands.erase(uid);
    m_mtx.unlock();
}


} //namespace ilrd
 


