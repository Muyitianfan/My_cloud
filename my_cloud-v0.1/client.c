/*************************************************************************
*File Name: client.c
*Author: TianFan
*mail: whywhy303918@Gmail.com
*Created Time: 2023年04月20日 星期四 18时35分40秒
 ************************************************************************/
#include "tcp.h"

int main(int argc, const char *argv[]){
    int ret,fd;
    char buf[BUFSIZ] = {"===test==="};
    /* 检测参数 */
    Argment(argc,argv);
    
    fd = SocketInit(argv);
    /* 发送客户端数据 */
    while(1){
        do{
            ret = send(fd,buf,BUFSIZ,0);
        }while(ret < 0 && errno == EINTR);
        if(ret < 0)
            ErrExit("recv");
        else if(!ret)
            break;
        printf("send data=%s\n",buf);
        getchar();
    }

    close(fd);

    return 0;
}

