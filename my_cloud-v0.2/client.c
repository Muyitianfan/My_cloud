/*************************************************************************
*File Name: client.c
*Author: TianFan
*mail: whywhy303918@Gmail.com
*Created Time: 2023年04月20日 星期四 16时35分40秒
 ************************************************************************/
#include "tcp.h"

int main(int argc, const char *argv[]){
    int ret,fd;
    
    char buf[BUFSIZ] = {"===test==="};
    /* 检测参数 */
    Argment(argc,argv);
    
    fd = SocketInit(argv,CLIENT_FLAG);
    /* 发送客户端数据 */
    while(1){
        ret = SocketDataHandle(fd,buf,strlen(buf),(DataHand_t)send);
        if(!ret)
            break;
        printf("send data:%s",buf);
        fflush(stdout);
        getchar();
    }

    close(fd);

    return 0;
}

