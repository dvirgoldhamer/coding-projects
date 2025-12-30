#ifndef _NETWORK_UTIL_H_
#define _NETWORK_UTIL_H_

#define _POSIX_C_SOURCE 200809L

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUF_LEN 100
#define MSG_LEN 5
#define BACKLOG 10

void InitAddrInfo(struct addrinfo* hints, int socket_type, int flag);

int GetSocketCandidates(const char* node, const char* port, struct addrinfo* hints, struct addrinfo** res);

//status_t CreateTCPSocket(char* who_am_i, int* sockfd, struct addrinfo** res);

int CreateUDPSocket(char* who_am_i, int* sockfd, struct addrinfo** res);

#endif
