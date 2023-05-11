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

int SocketInit(const char *argv[],bool flag){
    int fd;
    Addr_in addr;
    func_t func = flag ? bind: connect;
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
    if(flag){
        /* 地址快速重用 */
        int b_reuse = 1;
        setsockopt(fd, SOL_SOCKET,SO_REUSEADDR, &b_reuse, sizeof(int));
    }
    /* 发起连接请求或绑定地址 */
    if( func(fd,(Addr *)&addr,sizeof(addr)) )
        ErrExit("connect or bind");
    if(flag){
        /* 监听模式 */
        if(listen(fd,BACKLOG))
            ErrExit("linsten");
    }
    return fd;
}
int SocketDataHandle(int fd, void *buf, size_t len, DataHand_t datahandle){
    int ret;
    char *str = datahandle == recv ? "recv" : "send";
    do{
        ret = datahandle(fd,buf,len,0);
    }while(ret < 0 && errno == EINTR);
    if(ret < 0) {
        ErrExit(str);
    }
    return ret;
    
}
