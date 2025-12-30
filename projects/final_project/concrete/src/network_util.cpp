
#include "network_util.hpp"

#include <cstring> /*strcmp*/

void InitAddrInfo(struct addrinfo* hints, int socket_type, int flag)
{
    memset(hints, 0, sizeof(*hints));
    
    /*IPv4*/
    hints->ai_family = AF_INET;
    hints->ai_socktype = socket_type;
    hints->ai_flags = flag;
    
    
}

int GetSocketCandidates(const char* node, const char* port, struct addrinfo* hints, struct addrinfo** res)
{
    if ((getaddrinfo(node, port, hints, res) != 0))  
    {
        perror("created addres info failed");
        return -1;
    }
    
    return 0;
} 

int CreateUDPSocket(char* who_am_i, int* sockfd, struct addrinfo** res)

{
    struct addrinfo* curr_socket = {0};
    int broadcast = 1;

    for (curr_socket = *res; curr_socket != NULL; curr_socket =  
         curr_socket->ai_next)
    {
        if ((*sockfd = socket(curr_socket->ai_family,curr_socket->ai_socktype, 
             curr_socket->ai_protocol)) 
            == -1) 
        {
            continue;
        }
                
    if (setsockopt(*sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast,
    sizeof broadcast) == -1) 
    {
        perror("set BROADCAST failed");
        return -1;
    }

    //only server needs to bind
    if (!strcmp(who_am_i, "server"))
    {
        if (bind(*sockfd, curr_socket->ai_addr, 
        curr_socket->ai_addrlen) == -1)
        {   
            close(*sockfd);
            perror("binding failed");
            continue;
        }
    }
        break;
    }

    if (curr_socket == NULL)
    {
        perror("failed to create socket");
        return -1;

    }

    return 0;
}

