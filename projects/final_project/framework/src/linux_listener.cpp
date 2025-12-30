/*name: Dvir Goldhamer

File Name: linux_listener.cpp

purpose: implementation for a linux_listener

reviewed by

Date: */

#include <sys/select.h> 
#include <iostream> // cerr

#include "linux_listener.hpp"

namespace ilrd
{

void LinuxListener::SetWorkingSets(std::vector<std::pair<int,details::Mode>>& fds, fd_set& reading_set, fd_set& writing_set, size_t size, int& max_fd)
{
    FD_ZERO(&reading_set);
    FD_ZERO(&writing_set);

    for (size_t i = 0; i < size; ++i)
    {
        if (fds[i].second == details::Mode::READ)
        {
            FD_SET(fds[i].first, &reading_set);
        }
        else
        {
            FD_SET(fds[i].first, &writing_set);
        }
        max_fd = fds[i].first > max_fd ? fds[i].first : max_fd;
    }
}

std::vector<std::pair<int, details::Mode>> LinuxListener::Listen(std::vector<std::pair<int,details::Mode>>& fds) 
{
    int max_fd = 0;
    size_t size = fds.size(); 
    fd_set reading_set = {0}, writing_set = {0};
    
    SetWorkingSets(fds, reading_set, writing_set, size, max_fd);
    int num_of_modified;
    if ((num_of_modified = select(max_fd + 1, &reading_set, &writing_set, NULL, NULL)) <= 0)
    {
        std::cerr << "select failed" << std::endl;
        return {};
    } 
    
    std::vector<std::pair<int, details::Mode>> modified_fds;
    
    for(size_t i = 0; i < size && num_of_modified > 0; ++i)
    {
        if (FD_ISSET(fds[i].first, &reading_set) || FD_ISSET(fds[i].first, &writing_set)) //if fd was updated
        {   
            modified_fds.push_back(fds[i]);
            --num_of_modified;
        }
    }

    return modified_fds;
}

}