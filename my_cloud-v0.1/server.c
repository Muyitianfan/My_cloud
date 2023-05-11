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
    Addr_in addr,clinet_addr;
    socklen_t addrlen = sizeof(addr);
    /* 检测参数 */
    if(argc < 3){
        fprintf(stderr,"%s <addr><port>\n",argv[0]);
        exit(EXIT_FAILURE);
    }
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
    /* 绑定通信结构体 */
    if( bind(fd,(Addr *)&addr,sizeof(addr)) )
        ErrExit("bind");
    /* 监听模式 */
    if(listen(fd,BACKLOG))
        ErrExit("linsten");
    /* 接收客户端连接 */
    do{
        newfd = accept(fd,(Addr *)&clinet_addr,&addrlen);
    }while(newfd < 0 && errno == EINTR);//如果信号导致的错误，继续执行
    if(newfd < 0)
        ErrExit("accept");
    /* 接收客户端数据 */
    while(1){
        do{
            ret = recv(newfd,buf,BUFSIZ,0);
        }while(ret < 0 && errno == EINTR);
        if(ret < 0)
            ErrExit("recv");
        else if(!ret)
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

