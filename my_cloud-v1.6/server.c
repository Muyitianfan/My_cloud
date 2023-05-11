/*************************************************************************
*File Name: server.c
*Author: TianFan
*mail: whywhy303918@Gmail.com
*Created Time: 2023年04月23日 星期日 22时40分21秒
 ************************************************************************/
#include "filetransfer.h"
void Client_Handle(int fd, struct file_transfer *ft);
int main(int argc, char *argv[]){
    int sockfd,fd;
    struct config conf;
    struct file_transfer *ft = malloc(sizeof(struct file_transfer));

    /* 配置ip和端口号 */
    IpInit(&conf);
    /* 初始化套接字 */
    sockfd = SocketInit(&conf,SERVER);
    while(1){
        /* 连接客户端 */
        fd = accept(sockfd,NULL,NULL);
        if(fd < 0)
            ErrExit("accept");
        Client_Handle(fd, ft);
        close(fd);
    }
    close(sockfd);
    return 0;
}
void Client_Handle(int fd, struct file_transfer *ft){
    char *err_str =  "Invalid option!";
    while(Read(fd,ft,SIZE_FT_TYP)>0){
        switch(ft->type){
        case TYPE_GET://处理获取文件的请求
            FT_GetFileHandler(fd, ft);
            break;
        case TYPE_PUT://处理上传文件的请求
            FT_PutFileHandler(fd, ft);
            break;
        case TYPE_LIST://处理获取文件列表的请求
            FT_FileListHandler(fd, ft);
            break;
        case TYPE_SYNC://同步文件信息
            FT_SyncHandler(fd, ft);
            break;
        default:
            Write(fd, err_str, strlen(err_str));
        }
    }
    return;
}

