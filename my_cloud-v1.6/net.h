/*************************************************************************
*File Name: net.h
*Author: TianFan
*mail: whywhy303918@Gmail.com
*Created Time: 2023年04月23日 星期日 21时02分13秒
 ************************************************************************/
#ifndef _NET_
#define _NET_

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <unistd.h>

#define ErrExit(msg) do { \
	fprintf(stderr, "[%s:%d] ", \
			__FUNCTION__, __LINE__); \
	perror(msg); \
	exit(EXIT_FAILURE); } while(0)

#define SERVER true
#define CLIENT false
#define BACKLOG 5

#define SIZE_IP_STR (16+1) //ipv4地址最大16个字符加1可以读取到/n
#define SIZE_PORT_STR 6

#define DEBUG 1

typedef struct sockaddr Addr;
typedef struct sockaddr_in Addr_in;

struct config{
   char ip[SIZE_IP_STR];
   char port[SIZE_PORT_STR];
};
/* 配置ip端口号 */
void IpInit(struct config *conf);
/* 初始化套接字 */
int SocketInit(struct config *conf,bool S_or_C);

ssize_t Read(int fd, void *buf, size_t len);
ssize_t Write(int fd, const void *buf, size_t len);
#endif

