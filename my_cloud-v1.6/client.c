/*************************************************************************
*File Name: client.c
*Author: TianFan
*mail: whywhy303918@Gmail.com
*Created Time: 2023年04月23日 星期日 15时59分27秒
 ************************************************************************/
#include "filetransfer.h"
void usage(int argc, char *argv[]);

int main(int argc, char *argv[]){
    char buf[BUFSIZ];
    struct config conf;
    struct file_transfer *ft = (struct file_transfer *)buf;
    usage(argc,argv);
    /*根据config初始化环境*/
    IpInit(&conf);
    /*准备套接字*/
    int fd = SocketInit(&conf,CLIENT);
    
    switch(argv[1][1]) {
    case 'g': //获取文件
		FT_GetFile(fd, argv[2], ft);
		break;
	case 'p': //上传文件
		FT_PutFile(fd, argv[2], ft);
		break;
	case 'l'://获取文件列表
		FT_FileList(fd, ft);
		break;
	case 's'://同步文件信息
		FT_Sync(fd, ft);
		break;        
    default:
        fprintf(stderr,"无效的选项\n");

    }

    /*关闭文件描述符*/
    close(fd); 

    return 0;

}
void usage(int argc, char *argv[]){
    if(argc < 2 || argc > 4){
        fprintf(stderr, "%s[type]<argment>\n", argv[0]);
		fprintf(stderr, "\t[type]<filename> -g: get file.\n");
		fprintf(stderr, "\t[type]<filename> -p: put file.\n");
		fprintf(stderr, "\t[type] -l: upload file list\n");
		fprintf(stderr, "\t[type] -s: File synchronization.\n");
		exit(EXIT_FAILURE);
    }
    if((argv[1][1] == TYPE_PUT || argv[1][1]== TYPE_GET) && argc < 3){
        fprintf(stderr, "\t[type]<filename> -g: get file.\n");
		fprintf(stderr, "\t[type]<filename> -p: put file.\n");
		exit(EXIT_FAILURE);
    }
}
