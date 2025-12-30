
#include <iostream>

#include "udp_client_server.hpp"
#include "raid_manager.hpp"
#include "messages.hpp"
#include "factory.hpp"
#include "handleton.hpp"
#include "master_proxy.hpp"
#include "Minion_commands.hpp"
#include "framework.hpp"

using namespace ilrd;

int main()
{
    auto messageFactory = Handleton<factory_details::Factory<int, MessageBase>>::GetInstance();

    Handleton<FileManager>::GetInstance()->AddFilePath("/home/dvir-goldhamer/git/projects/final_project/data.bin");

    messageFactory->Register(READ_MESSAGE, ReadMessage::CreateReadMessage);
    messageFactory->Register(WRITE_MESSAGE, WriteMessage::CreateWriteMessage);

    struct addrinfo* res;
    int sockfd = UDPFunctionality::CreateSocket("server", &res, RAIDManager::MINION1_IP_ADRRESS, RAIDManager::MINION1_PORT);
    
    std::unordered_map<FdModeWrapper,                  std::shared_ptr<interfaces::IInputProxy>, FdModeHash> proxy_map;

    proxy_map[{sockfd, READ}] = std::make_shared<MasterProxy>();

    std::unordered_map<int, std::function<std::shared_ptr<interfaces::ICommand>()>> creators;

    //factory
    creators[READ] = CreateMinionReadCommand;
    creators[WRITE] = CreateMinionWriteCommand;

    Framework framework(proxy_map,
                        creators,
                        "/home/dvir-goldhamer");
    
    framework.Run();//Blocking

    return 0;
}