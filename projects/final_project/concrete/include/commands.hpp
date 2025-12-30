/*name: Dvir Goldhamer

File Name: commands.hpp

purpose: API for read and write commands

reviewed by : 

Date: */

#ifndef IRLD_RD168_169_COMMANDS_HPP
#define IRLD_RD168_169_COMMANDS_HPP

#include "framework_interfaces.hpp"
#include "raid_manager.hpp"
#include "response_manager.hpp"
#include "unordered_map"

#include <iostream>

namespace ilrd
{

class ReadCommand : public interfaces::ICommand
{
    std::pair<std::function<bool()>,std::chrono::milliseconds> Execute(std::shared_ptr<interfaces::IKeyArgs> key_args)
    {
        std::pair commands_uids = Handleton<RAIDManager>::GetInstance()->Read(key_args);
         
        auto async_pair = std::make_pair([commands_uids]() 
        {
            return interfaces::ICommand::AsyncFunc(commands_uids); 
        },
        std::chrono::milliseconds(100));

        return async_pair;
    }
};

std::shared_ptr<interfaces::ICommand> CreateReadCommand()
{
    return std::make_shared<ReadCommand>();
}

class WriteCommand : public interfaces::ICommand
{
    std::pair<std::function<bool()>,std::chrono::milliseconds> Execute(std::shared_ptr<interfaces::IKeyArgs> key_args)
    {
        std::pair commands_uids = Handleton<RAIDManager>::GetInstance()->Write(key_args);

        auto async_pair = std::make_pair([commands_uids]() 
        {
            return interfaces::ICommand::AsyncFunc(commands_uids); 
        },
        std::chrono::milliseconds(100));
        
        return async_pair;
    }
};

std::shared_ptr<interfaces::ICommand> CreateWriteCommand()
{
    return std::make_shared<WriteCommand>();
}

bool interfaces::ICommand::AsyncFunc(std::pair<UID,UID> commands_uids)
{
    static int counter = 0;

    auto response_manager = Handleton<ResponseManager>::GetInstance();

    std::pair<STATUS, STATUS> commands_status;

    commands_status.first = response_manager->GetCommandStatus(commands_uids.first);

    commands_status.second = response_manager->GetCommandStatus(commands_uids.second);

    ++counter;

    //if status of any of commands isnt success, resampling will occur.
    //resampling will stop after 10 times.

    if (counter == 10 || ((commands_status.first == SUCCESS) && (commands_status.second == SUCCESS)))
    {
        if (commands_status.first == SUCCESS)
        {
            response_manager->UpdateStatus(commands_uids.first ,SUCCESS);
        }
        else if (counter == 10)
        {
            response_manager->UpdateStatus(commands_uids.first ,COMPLETE_FAILURE);
        }

        response_manager->DeleteCommand(commands_uids.first);
        response_manager->DeleteCommand(commands_uids.second);

        counter = 0;
        return false;
    }
    else
    {
        return true;
    }
} 

} //namespace ilrd

#endif //IRLD_RD168_169_COMMANDS_HPP