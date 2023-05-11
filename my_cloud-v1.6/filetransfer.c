/*************************************************************************
*File Name: TF_FileTransfer.c
*Author: TianFan
*mail: whywhy303918@Gmail.com
*Created Time: 2023年04月24日 星期一 16时24分21秒
 ************************************************************************/
#include "filetransfer.h"

int Occupation_Space(char *f_name,long long f_size){
    char buf[BUFSIZ] = {};
    int f_fd;
    if((f_fd = open(f_name, O_WRONLY|O_CREAT|O_TRUNC, 0640)) < 0){
        ErrExit("open");
        return 0;
    }
    while(f_size > 0){
        if(f_size > BUFSIZ){
            if(Write(f_fd, buf, BUFSIZ) == -1)
                return -1;
            f_size -= BUFSIZ;
        } else {
            if(Write(f_fd, buf, f_size) == -1)
                return -1;
            f_size -= f_size;
        }
    }    
    close(f_fd);
    return 0;
}

void Send_File(int sockfd, char *f_name, long long f_size){
    char buf[BUFSIZ] = {};
    int f_fd;
    if((f_fd = open(f_name,O_RDONLY)) < 0){
        ErrExit("open");
        return;
    }
    ssize_t num_read;
    while(f_size > 0){
        if(f_size > BUFSIZ){
            num_read = Read(f_fd, buf,BUFSIZ);
            Write(sockfd , buf, num_read);
            f_size -= num_read;
        } else {
            num_read = Read(f_fd, buf, f_size);
            Write(sockfd, buf, num_read);
            f_size -= num_read;
        }
    }
    close(f_fd);
    return;
}
void Receive_File(int sockfd, char *f_name, long long f_size){
    char buf[BUFSIZ] = {};
    int f_fd;
    if((f_fd = open(f_name, O_WRONLY|O_TRUNC)) < 0){
        ErrExit("open");
        return;
    }
    ssize_t num_read;
    while(f_size > 0){
        if(f_size > BUFSIZ){
            num_read = Read(sockfd, buf,BUFSIZ);
            Write(f_fd, buf, num_read);
            f_size -= num_read;
        } else {
            num_read = Read(sockfd, buf, f_size);
            Write(f_fd, buf, num_read);
            f_size -= num_read;
        }
    }
    close(f_fd);
    return;
}

/********************  客户端函数  ********************/
/*获取文件*/
void FT_GetFile(int sockfd, char *f_name, struct file_transfer *ft){
    printf("[%s:%d]\n",__FUNCTION__,__LINE__);
    /* 发送获取文件的请求 */
    ft->type = TYPE_GET;
    Write(sockfd,ft,SIZE_FT_TYP);
    /* 发送文件名字 */    
    ft->len = strlen(f_name);
    Write(sockfd, &ft->len, 1);
    Write(sockfd, f_name, ft->len);
    /* 收到服务器确认信息 */
    Read(sockfd, ft,SIZE_FT_TYP);
    if(ft->type == TYPE_ERR){
        printf("error\n");
    } else if(ft->type == TYPE_OK){
        /* 读取文件大小 */
        Read(sockfd, &ft->f_size, SIZE_FT_SIZE);
        printf("%lld\n",ft->f_size);
        /* 占领文件空间 */
        if( Occupation_Space(f_name,ft->f_size) == 0){//如果成功发送确认请求
            ft->type = TYPE_OK;
            Write(sockfd, ft, SIZE_FT_TYP);
            Receive_File(sockfd,f_name,ft->f_size);
        }
        else {
            ft->type = TYPE_ERR;
            Write(sockfd,ft,SIZE_FT_TYP);
            printf("内存不足，取消下载\n");
        }
    }
    return;
}
/*上传文件*/
void FT_PutFile(int sockfd, char *f_name, struct file_transfer *ft){
    printf("[%s:%d]\n",__FUNCTION__,__LINE__);
    bool flag;
    /* 判断文件类型 */
    struct stat st;
    if((stat(f_name, &st) == 0) 
       && S_ISREG(st.st_mode) 
       && f_name[0] != '.'){  //判断文件是否存在，且为常规文件
        /* 发送上传文件请求 */
        ft->type = TYPE_PUT;
        Write(sockfd,ft,SIZE_FT_TYP);

        /* 发送文件名字 */    
        ft->len = strlen(f_name);
        Write(sockfd, &ft->len, 1);
        Write(sockfd, f_name, ft->len);
        /* 收到服务器确认信息 */
        Read(sockfd, ft,SIZE_FT_TYP);
        if(ft->type == TYPE_OK){
            flag = true;
        } else {
            /* 接收文件属性 */
            Read(sockfd,&ft->f_time,SIZE_FT_TIM);
            Read(sockfd,&ft->f_size,SIZE_FT_SIZE);
            ft->f_time = st.st_mtime;
            ft->f_size = st.st_size;

            struct tm * ctimestr;
            ctimestr = localtime(&ft->f_time);
            printf("文件信息:\n");
            printf("%lld %d月 %02d %02d:%02d %s\n", ft->f_size, ctimestr->tm_mon+1, ctimestr->tm_mday,ctimestr->tm_hour, ctimestr->tm_min ,f_name);
            printf("输入y确认上传\n");
            if(getchar() == 'y'){
                /* 发送是否上传请求 */
                printf("文件上传中\n");
                ft->type = TYPE_OK;
                Write(sockfd,&ft->type, SIZE_FT_TYP);
                flag = true;
            } else {
                /* 发送是否上传请求 */
                ft->type = TYPE_ERR;
                Write(sockfd,&ft->type, SIZE_FT_TYP);
                flag = true;
                return ;
            }
        }
    } else {
        printf("该文件不存在或不是常规文件\n");
    }
    if(flag){
        /* 收到OK文件不存在可以之直接上传 */
        ft->f_size = st.st_size;
        Write(sockfd, &ft->f_size,SIZE_FT_SIZE);
        Read(sockfd, ft,SIZE_FT_TYP);
        if(ft->type == TYPE_OK){
            Send_File(sockfd, f_name, ft->f_size);
            printf("文件传输完成\n");
        } else {
            printf("云盘空间不足，上传失败\n");
        }
    }

    return;
}
/*获取文件列表*/
void FT_FileList(int sockfd, struct file_transfer *ft){
    printf("[%s:%d]\n",__FUNCTION__,__LINE__);
    ft->type = TYPE_LIST;
    Write(sockfd,ft,SIZE_FT_TYP);
    Read(sockfd,ft,SIZE_FT_TYP);
    if(ft->type == TYPE_OK){
        read(sockfd, &ft->f_size, SIZE_FT_SIZE);
        Occupation_Space(DIR_FILE,ft->f_size);
        Receive_File(sockfd, DIR_FILE, ft->f_size);
    } else {
        printf("文件列表获取失败");
        return;
    }
    FILE *fp;
    char buf[BUFSIZ];
    fp = fopen(DIR_FILE, "r");
    if(fp == NULL){   
        ErrExit("fopen");
        return;
    }
    while (fgets(buf, BUFSIZ, fp) != NULL) {
        printf("%s", buf);
    }
    fclose(fp);
    

    return;
}
/*同步文件信息*/
void FT_Sync(int sockfd, struct file_transfer *ft){
    printf("[%s:%d]\n",__FUNCTION__,__LINE__);
    bool flag;
    ft->type = TYPE_SYNC;
    Write(sockfd,ft,SIZE_FT_TYP);

    Read(sockfd,&ft->type, SIZE_FT_TYP);
    while(ft->type == TYPE_OK){
        /* 发送文件属性 */
        Read(sockfd,&ft->len, 1);
        Read(sockfd,ft->f_name,ft->len);
        Read(sockfd,&ft->f_time,SIZE_FT_TIM);
        Read(sockfd,&ft->f_size,SIZE_FT_SIZE);
        ft->f_name[ft->len] = '\0';
        printf("%s\n",ft->f_name);
        struct stat st;
        if((stat(ft->f_name, &st) == 0)){  //判断文件是否存在
            if(ft->f_size == st.st_size && ft->f_time == st.st_mtime){//判断属性是否一样；
                printf("云端内容相同与本地内容一样，无须下载%s\n",ft->f_name);
                flag = false;
            } else {
                flag = true;
            }
        } else {
            flag = true;
        }
        if(flag == true){
            ft->type = TYPE_OK;
            Write(sockfd, ft, SIZE_FT_TYP);
            Occupation_Space(ft->f_name, ft->f_size);
            Receive_File(sockfd, ft->f_name, ft->f_size);
        } else {
            ft->type = TYPE_ERR;
            Write(sockfd,ft,SIZE_FT_TYP);
        }

        Read(sockfd, &ft->type,SIZE_FT_TYP);
    }

    ft->type = TYPE_ERR;
    Write(sockfd,ft,SIZE_FT_TYP);



    return;
}
/********************  服务端函数  ********************/
/*处理获取文件的请求*/
void FT_GetFileHandler(int sockfd, struct file_transfer *ft){
    printf("[%s:%d]\n",__FUNCTION__,__LINE__);
    

    /* 接受文件名字 */
    Read(sockfd, &ft->len, 1);
    Read(sockfd, ft->f_name, ft->len);
    ft->f_name[ft->len] = '\0';
    printf("[%s]\n",ft->f_name);
    /* 判断文件类型 */
    struct stat st;
    if((stat(ft->f_name, &st) == 0) 
       && S_ISREG(st.st_mode) 
       && ft->f_name[0] != '.'){  //判断文件是否存在，且为常规文件
        ft->type = TYPE_OK;
        Write(sockfd,ft,SIZE_FT_TYP);
        
        ft->f_size = st.st_size;
        /* 发送文件大小 */
        Write(sockfd, &ft->f_size ,SIZE_FT_SIZE);
        /* 收到确认空间请求 */
        Read(sockfd, ft, SIZE_FT_TYP);
        /* 收到下载文件请求 */
        if(ft->type == TYPE_OK)
            Send_File(sockfd, ft->f_name, ft->f_size);
    } else {
        ft->type = TYPE_ERR;
        Write(sockfd,ft,SIZE_FT_TYP);
        return;
    }

    return;
}
/*处理上传文件的请求*/
void FT_PutFileHandler(int sockfd, struct file_transfer *ft){
    bool flag;
    printf("[%s:%d]\n",__FUNCTION__,__LINE__);
    /* 接受文件名字 */
    Read(sockfd, &ft->len, 1);
    Read(sockfd, ft->f_name, ft->len);
    printf("%d\n",ft->len);
    ft->f_name[ft->len] = '\0';
    printf("[%s]\n",ft->f_name);
    /* 判断文件类型 */
    struct stat st;
    if((stat(ft->f_name, &st) == 0) && S_ISREG(st.st_mode)){  //判断文件是否存在，且为常规文件
        /* 文件存在发送ERR */
        ft->type = TYPE_ERR;
        Write(sockfd,&ft->type,SIZE_FT_TYP);
        /* 发送文件属性 */
        ft->f_time = st.st_mtime;
        ft->f_size = st.st_size;
        Write(sockfd,&ft->f_time,SIZE_FT_TIM);
        Write(sockfd,&ft->f_size,SIZE_FT_SIZE);
        /* 获取是否接收请求 */
        Read(sockfd,&ft->type, SIZE_FT_TYP);
        if(ft->type == TYPE_OK){
            flag = true;
        } else {
            flag = false;
        }
    } else {
        /* 文件不存在发送OK */
        ft->type = TYPE_OK;
        Write(sockfd,ft,SIZE_FT_TYP);
        flag = true;

    }
    if(flag){
        /* 获取文件大小 */
        Read(sockfd, &ft->f_size, SIZE_FT_SIZE);
        /* 占领空间 */
        if(Occupation_Space(ft->f_name,ft->f_size) == 0){
            /* 占领成功发送确认 */
            ft->type = TYPE_OK;
            Write(sockfd,ft,SIZE_FT_TYP);
            /* 接收文件 */
            Receive_File(sockfd, ft->f_name, ft->f_size);
        } else {
            /* 占领失败发送错误 */
            ft->type = TYPE_ERR;
            Write(sockfd,ft,SIZE_FT_TYP);
            /* 删除错误文件 */
        }
    }

    return;
}
/*处理获取文件列表的请求*/
void FT_FileListHandler(int sockfd, struct file_transfer *ft){
    printf("[%s:%d]\n",__FUNCTION__,__LINE__);

    DIR * dir;
    int f_fd;
    struct dirent *entry;
    /* 打开文件列表文件 */
    if((f_fd = open(DIR_FILE, O_WRONLY|O_CREAT|O_TRUNC, 0640)) < 0){   
        ErrExit("open");
        return;
    }
    dir = opendir(".");
    while( (entry = readdir(dir)) != NULL){
        strcpy(ft->f_name,entry->d_name);
        if(ft->f_name[0] != '.' && entry->d_type ==DT_REG){
            printf("%s\n",ft->f_name);
            Write(f_fd,ft->f_name,strlen(ft->f_name));
            Write(f_fd,"\n",1);
        }
    }
    /* 判断文件类型 */
    struct stat st;
    stat(DIR_FILE, &st);
    ft->f_size = st.st_size;

    /* 读取目录成功发送确认 */
    ft->type = TYPE_OK;
    Write(sockfd,ft,SIZE_FT_TYP);
    /* 发送目录 */
    Write(sockfd,&ft->f_size,SIZE_FT_SIZE);
    Send_File(sockfd, DIR_FILE, ft->f_size);
    
    
    close(f_fd);
    return;
}
/*同步文件信息*/
void FT_SyncHandler(int sockfd, struct file_transfer *ft){
    printf("[%s:%d]\n",__FUNCTION__,__LINE__);
    DIR * dir;
    struct dirent *entry;
    dir = opendir(".");
    while( (entry = readdir(dir)) != NULL){
        strcpy(ft->f_name,entry->d_name);
        if(ft->f_name[0] != '.' && entry->d_type ==DT_REG){
            /* 读取目录成功发送确认 */
            ft->type = TYPE_OK;
            Write(sockfd,ft,SIZE_FT_TYP);
            /* 发送文件属性 */
            ft->len = strlen(ft->f_name);
            struct stat st;
            stat(ft->f_name, &st);
            ft->f_size = st.st_size;    
            ft->f_time = st.st_mtime;
            Write(sockfd,&ft->len, 1);
            Write(sockfd,ft->f_name,ft->len);
            Write(sockfd,&ft->f_time,SIZE_FT_TIM);
            Write(sockfd,&ft->f_size,SIZE_FT_SIZE);
            /* 收到确认信息 */
            Read(sockfd, &ft->type,SIZE_FT_TYP);
            if(ft->type == TYPE_OK){
                Send_File(sockfd, ft->f_name, ft->f_size);    
            } 
        }

    }
    ft->type = TYPE_ERR;
    Write(sockfd,ft,SIZE_FT_TYP);
    return;
}



