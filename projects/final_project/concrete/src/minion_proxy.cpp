/*name: Dvir Goldhamer

File Name: minion_proxy.cpp

purpose: Implementation for a minion proxy class

reviewed by :

Date: */

#include <iostream>
#include <cstring>

#include "udp_client_server.hpp"
#include "minion_proxy.hpp"
#include "key_args.hpp"
#include "messages.hpp"
#include "response_manager.hpp"
#include "raid_manager.hpp"

namespace ilrd
{

MinionProxy::MinionProxy(const std::string ip_address, const std::string port) :  m_sockfd(UDPFunctionality::CreateSocket("client", &m_res, ip_address, port))
{
}

UID MinionProxy::Read(std::shared_ptr<interfaces::IKeyArgs> key_args)
{
    auto read_args = std::static_pointer_cast<ReadArgs>(key_args);

    ReadMessage r_message(read_args->GetOffset(), read_args->GetHowMany());
    uint64_t length = r_message.GetMessageLength();
    auto msg_buffer = std::make_shared<char[]>(length);
    r_message.ToBuffer(msg_buffer.get());


    //adding command to the response manager map
    Handleton<ResponseManager>::GetInstance()->AddCommand(read_args->GetID(), r_message.GetUID());

    //sending buffer to minion
    UDPFunctionality::Send(m_sockfd, m_res, msg_buffer.get(), length);

    return (r_message.GetUID());

}

UID MinionProxy::Write(std::shared_ptr<interfaces::IKeyArgs> key_args)
{
    auto write_args = std::static_pointer_cast<WriteArgs>(key_args);

    WriteMessage w_message(write_args->GetOffset(), write_args->GetHowMany(), write_args->GetBuffer());

    uint64_t length = w_message.GetMessageLength();
    
    auto msg_buffer = std::make_shared<char[]>(length);
    w_message.ToBuffer(msg_buffer.get());

    //adding command to the response manager map
    Handleton<ResponseManager>::GetInstance()->AddCommand(write_args->GetID(), w_message.GetUID());
    
    //sending buffer to minion
    UDPFunctionality::Send(m_sockfd, m_res, msg_buffer.get(), length);

    return (w_message.GetUID());
}

} //namespace ilrd