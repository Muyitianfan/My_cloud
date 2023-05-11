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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>

#define ErrExit(msg) do { perror(msg); exit(EXIT_FAILURE); }while(0)
#define BACKLOG 5
typedef struct sockaddr Addr;
typedef struct sockaddr_in Addr_in;

void Argment(int argc, const char *argv[]);
int SocketInit(const char *argv[]);
#endif

