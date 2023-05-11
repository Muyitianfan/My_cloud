/*************************************************************************
*File Name: tcp.h
*Author: TianFan
*mail: whywhy303918@Gmail.com
*Created Time: 2023年04月20日 星期四 16时37分28秒
 ************************************************************************/
#ifndef _TCP_H
#define _TCP_H

#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>

#define ErrExit(msg) do { perror(msg); exit(EXIT_FAILURE); }while(0)

#define BACKLOG 5
#define SERVER_FLAG true
#define CLIENT_FLAG false 

typedef struct sockaddr Addr;
typedef struct sockaddr_in Addr_in;

typedef int (* func_t)(int , const Addr*,socklen_t);
typedef ssize_t(* DataHand_t)(int , void *, size_t ,int);
void Argment(int argc, const char *argv[]);
int SocketInit(const char *argv[],bool flag); 
int SocketDataHandle(int fd, void *buf, size_t len, DataHand_t datahandle);
#endif

