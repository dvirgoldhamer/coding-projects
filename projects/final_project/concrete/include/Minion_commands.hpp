/*name: Dvir Goldhamer

File Name: minion_commands.hpp

purpose: API for read and write commands for the minion

reviewed by : 

Date: */

#ifndef IRLD_RD168_169_MINION_COMMANDS_HPP
#define IRLD_RD168_169_MINION_COMMANDS_HPP

#include <fstream>
#include <iostream>

#include "minion_key_args.hpp"
#include "framework_interfaces.hpp"
#include "file_manager.hpp"
#include "master_proxy.hpp"

namespace ilrd
{

class MinionReadCommand : public interfaces::ICommand
{
    std::pair<std::function<bool()>,std::chrono::milliseconds> Execute(std::shared_ptr<interfaces::IKeyArgs> key_args)
    {

        auto read_args = std::static_pointer_cast<MinionReadArgs>(key_args);

        auto buffer = Handleton<FileManager>::GetInstance()->ReadFromFile( read_args);

        STATUS read_status = SUCCESS;
        if (!buffer)
        {
            read_status = FAILURE;
        }
    
        MasterProxy::SendReadResponse(buffer, read_args->GetHowMany(), read_status, read_args->GetUID());

    return std::make_pair(nullptr, std::chrono::milliseconds(100));    
    }
};

std::shared_ptr<interfaces::ICommand> CreateMinionReadCommand()
{
    return std::make_shared<MinionReadCommand>();
}

class MinionWriteCommand : public interfaces::ICommand
{
     std::pair<std::function<bool()>,std::chrono::milliseconds> Execute(std::shared_ptr<interfaces::IKeyArgs> key_args)
    {
        auto write_args = std::static_pointer_cast<MinionWriteArgs>(key_args);

        STATUS write_status = Handleton<FileManager>::GetInstance()->WriteToFile(write_args);
        
        MasterProxy::SendWriteResponse(write_status, write_args->GetUID());

        return std::make_pair(nullptr, std::chrono::milliseconds(100));    

    }
};

std::shared_ptr<interfaces::ICommand> CreateMinionWriteCommand()
{
    return std::make_shared<MinionWriteCommand>();
}

} //namespace ilrd

#endif //IRLD_RD168_169_MINION_COMMANDS_HPP