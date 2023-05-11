/*************************************************************************
*File Name: server.c
*Author: TianFan
*mail: whywhy303918@Gmail.com
*Created Time: 2023年04月20日 星期四 16时35分40秒
 ************************************************************************/
#include "tcp.h"

int main(int argc, const char *argv[]){
    int fd,newfd;
    int ret;
    char buf[BUFSIZ];
    Addr_in clinet_addr;
    socklen_t addrlen = sizeof(clinet_addr);
    /* 检测参数 */
    Argment(argc,argv);
    /* 创建服务端套接字 */
    fd = SocketInit(argv,SERVER_FLAG);
    /* 接收客户端连接 */
    do{
        newfd = accept(fd,(Addr *)&clinet_addr,&addrlen);
    }while(newfd < 0 && errno == EINTR);//如果信号导致的错误，继续执行
    if(newfd < 0)
        ErrExit("accept");
    /* 接收客户端数据 */
    while(1){
        ret = SocketDataHandle(newfd, buf, BUFSIZ, recv);
        if(!ret)
            break;
        else
            printf("[%s:%d]buf:%s\n",
                   inet_ntoa(clinet_addr.sin_addr),
                   ntohs(clinet_addr.sin_port),buf);
    }

    close(newfd);
    close(fd);

    return 0;
}

