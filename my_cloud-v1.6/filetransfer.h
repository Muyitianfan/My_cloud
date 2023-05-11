/*************************************************************************
*File Name: filetransfer.h
*Author: TianFan
*mail: whywhy303918@Gmail.com
*Created Time: 2023年04月25日 星期二 09时01分24秒
 ************************************************************************/

#ifndef _FILE_TRANSFER_H_
#define _FILE_TRANSFER_H_

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>

#include "net.h"
#define DIR_FILE ".dir_file"
#define SIZE_FT_TYP 1
#define SIZE_FT_SIZE sizeof(long long)
#define SIZE_FT_TIM sizeof(time_t)
enum {
    TYPE_ERR, TYPE_OK, TYPE_GET, TYPE_PUT, TYPE_LIST, TYPE_SYNC
};

struct file_transfer {
    unsigned int type;//标志位
    unsigned int  len;//名字长度
    char    f_name[256];//文件名字
    long       f_time;//最后修改时间
    long long   f_size;//文件大小
};

int Occupation_Space(char *f_name,long long f_size);
void Receive_File(int sockfd, char *f_name, long long f_size);
void Send_File(int sockfd, char *f_name, long long f_size);

/*获取文件*/
void FT_GetFile(int sockfd, char *f_name, struct file_transfer *ft);
/*上传文件*/
void FT_PutFile(int sockfd, char *f_name, struct file_transfer *ft);
/*获取文件列表*/
void FT_FileList(int sockfd, struct file_transfer *ft);
/*同步文件信息*/
void FT_Sync(int sockfd, struct file_transfer *ft);
/*********************************/

/**服务端函数**/
/*********************************/
/*处理获取文件的请求*/
void FT_GetFileHandler(int sockfd, struct file_transfer *ft);
/*处理上传文件的请求*/
void FT_PutFileHandler(int sockfd, struct file_transfer *ft);
/*处理获取文件列表的请求*/
void FT_FileListHandler(int sockfd, struct file_transfer *ft);
/*同步文件信息*/
void FT_SyncHandler(int sockfd, struct file_transfer *ft);

#endif
