/*************************************************************************
*File Name: tcp.c
*Author: TianFan
*mail: whywhy303918@Gmail.com
*Created Time: 2023年04月21日 星期五 11时48分15秒
 ************************************************************************/
#include "tcp.h"


void Argment(int argc, const char *argv[]){
    if(argc < 3){
        fprintf(stderr,"%s <addr><port>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
}

int SocketInit(const char *argv[]){
    int fd;
    Addr_in addr;
    /* 创建套结字 */
    if((fd = socket(AF_INET, SOCK_STREAM,0))<0)
        ErrExit("socket");
    /* 设置通信结构体 */
    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    if (inet_aton(argv[1], &addr.sin_addr) == 0) {
        fprintf(stderr, "Invalid address\n");
        exit(EXIT_FAILURE);
    }
    /* 发起连接请求 */
    if( connect(fd,(Addr *)&addr,sizeof(addr)) )
        ErrExit("connect");

    return fd;
}
