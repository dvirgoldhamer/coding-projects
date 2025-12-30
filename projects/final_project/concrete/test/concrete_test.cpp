/*name: Dvir Goldhamer

File Name: framework_test.cpp

purpose: test file for concrete commands

reviewed by : 

Date: */

#include <unistd.h> //STDFILENO
#include <utility> // std::pair, std::make_pair

#include "framework.hpp"
#include "raid_manager.hpp"
#include "minion_proxy.hpp"
#include "commands.hpp"
#include "key_args.hpp"
#include "messages.hpp"
#include "udp_client_server.hpp"
#include "response_proxy.hpp"
#include "nbd_proxy.hpp"

using namespace ilrd;

#define RED ("\033[31m")
#define GREEN ("\033[32m")
#define BLUE ("\x1B[34m")
#define DEFAULT ("\033[0m")

    
int main()
{
    std::unordered_map<FdModeWrapper,                  std::shared_ptr<interfaces::IInputProxy>, FdModeHash> proxy_map;
    
    auto nbd_handler = Handleton<NBDHandler>::GetInstance();
   
    nbd_handler->Init();

    auto messageFactory = Handleton<factory_details::Factory<int, MessageBase>>::GetInstance();

    messageFactory->Register(READ_RESPONSE_MESSAGE, ReadResponseMessage::CreateReadResponseMessage);
    messageFactory->Register(WRITE_RESPONSE_MESSAGE, WriteResponseMessage::CreateWriteResponseMessage);

    // proxy_map[{STDIN_FILENO, READ}] = std::make_shared<NBDProxy>();

    proxy_map[{nbd_handler->GetSocketFd(), READ}] = std::make_shared<NBDProxy>();
  
    struct addrinfo* res;
    int sockfd = UDPFunctionality::CreateSocket("server", &res, RAIDManager::MASTER_IP_ADRRESS, RAIDManager::MASTER_PORT);

    proxy_map[{sockfd, READ}] = std::make_shared<ResponseProxy>();

    std::unordered_map<int, std::function<std::shared_ptr<interfaces::ICommand>()>> creators;

    creators[READ] = CreateReadCommand;
    creators[WRITE] = CreateWriteCommand;

    Framework framework(proxy_map,
                        creators,
                        "/home/dvir-goldhamer/git/projects/final_project/plugins");
    
    framework.Run();//Blocking

    return 0;
}