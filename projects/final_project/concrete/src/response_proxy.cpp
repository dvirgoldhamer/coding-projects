/*name: Dvir Goldhamer

File Name: response_proxy.cpp

purpose: Implementation for a repsponse proxy class

reviewed by :

Date: */

#include <iostream>

#include "udp_client_server.hpp"
#include "response_proxy.hpp"
#include "response_manager.hpp"
#include "factory.hpp"
#include "messages.hpp"

namespace ilrd
{

std::shared_ptr<interfaces::IKeyArgs> ResponseProxy::GetKeyArgs(int fd, ilrd::Mode mode)
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
    

    if(key == READ_RESPONSE_MESSAGE)
    {
        auto rr_message = std::static_pointer_cast<ReadResponseMessage>(message_base);
        
        std::cout << "read response message uid is " << rr_message->GetUID().m_counter << std::endl;
           std::cout << "read response message status is " << rr_message->GetStatus() << std::endl;
        
        Handleton<ResponseManager>::GetInstance()->UpdateStatus(rr_message->GetUID(), rr_message->GetStatus(), rr_message->GetBuffer(), rr_message->GetBufferSize());
    }

    if(key == WRITE_RESPONSE_MESSAGE)
    {
        auto wr_message = std::static_pointer_cast<WriteResponseMessage>(message_base);

        std::cout << "write response message uid is " << wr_message->GetUID().m_counter << std::endl;

            std::cout << "write response message status is " << wr_message->GetStatus() << std::endl;

        Handleton<ResponseManager>::GetInstance()->UpdateStatus
        (wr_message->GetUID(), wr_message->GetStatus());
    }

    return nullptr;

}

}//namespace ilrd
