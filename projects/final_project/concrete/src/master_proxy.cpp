/*name: Dvir Goldhamer

File Name: master_proxy.cpp

purpose: Implementation for a master proxy class

reviewed by :

Date: */

#include <iostream>
#include <cstring>

#include "udp_client_server.hpp"
#include "master_proxy.hpp"
#include "minion_key_args.hpp"
#include "messages.hpp"
#include "factory.hpp"
#include "handleton.hpp"
#include "raid_manager.hpp"

namespace ilrd
{


MasterProxy::MasterProxy() 
{
    m_sockfd = (UDPFunctionality::CreateSocket("client", &m_res, RAIDManager::MASTER_IP_ADRRESS, RAIDManager::MASTER_PORT));
}

std::shared_ptr<interfaces::IKeyArgs> MasterProxy::GetKeyArgs(int fd, Mode mode)
{
    (void)mode;

    std::shared_ptr<char[]> buffer = UDPFunctionality::Receive(fd, sizeof(uint64_t), MSG_PEEK);
    
    uint64_t length = *(uint64_t*)buffer.get();

    std::shared_ptr<char[]> data_buffer = UDPFunctionality::Receive(fd, length, MSG_WAITALL);

    char* buffer_copy = data_buffer.get();
    buffer_copy += sizeof(uint64_t); //move forward only length field
    
    KEY key = *(KEY*)buffer_copy;
    
    auto messageFactory = Handleton<factory_details::Factory<int, MessageBase>>::GetInstance();
    auto message_base = messageFactory->CreateObject(key);
    message_base->FromBuffer(data_buffer.get()); 
    
    if(key == READ_MESSAGE)
    {
        auto r_message = std::static_pointer_cast<ReadMessage>(message_base);
        
        return std::make_shared<MinionReadArgs>(r_message->GetHowMany(), r_message->GetOffset(), r_message->GetUID());
    }

    if(key == WRITE_MESSAGE)
    {
        auto w_message = std::static_pointer_cast<WriteMessage>(message_base);

        return std::make_shared<MinionWriteArgs>(w_message->GetHowMany(), w_message->GetOffset(), w_message->GetUID(), w_message->GetBuffer());
    }

    return nullptr;

}

void MasterProxy::SendReadResponse(std::shared_ptr<char[]> buffer, uint64_t buffer_length, STATUS read_status, UID uid)
{

    ReadResponseMessage rr_message(buffer, buffer_length, read_status, uid);

    uint64_t length = rr_message.GetMessageLength();
    auto msg_buffer = std::make_shared<char[]>(length);
    rr_message.ToBuffer(msg_buffer.get());

    UDPFunctionality::Send(m_sockfd, m_res, msg_buffer.get(), length);
}

void MasterProxy::SendWriteResponse(STATUS write_status, UID uid)
{

    std::cout << "write response in minion (master proxy), " << "uid is " << uid.m_counter << std::endl;

    WriteResponseMessage wr_message(write_status, uid);

    uint64_t length = wr_message.GetMessageLength();
    auto msg_buffer = std::make_shared<char[]>(length);
    wr_message.ToBuffer(msg_buffer.get());

    UDPFunctionality::Send(m_sockfd, m_res, msg_buffer.get(), length);
}



} //namespace ilrd