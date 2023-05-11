/*************************************************************************
*File Name: net.c
*Author: TianFan
*mail: whywhy303918@Gmail.com
*Created Time: 2023年04月23日 星期日 21时03分33秒
 ************************************************************************/
#include "net.h"

void IpInit(struct config *conf){
    size_t len;
	FILE *fp = fopen(".config", "r");
	if(fp == NULL)
		ErrExit("fopen");
	/*设置IP地址*/
	fgets(conf->ip, SIZE_IP_STR, fp);
	len = strlen(conf->ip);
	if(conf->ip[len-1] != '\0')
		conf->ip[len-1] = '\0';
	/*设置端口号*/
	fgets(conf->port, SIZE_IP_STR, fp);
	len = strlen(conf->port);
	if(conf->port[len-1] != '\0')
		conf->port[len-1] = '\0';
#ifdef DEBUG
	printf("[%s:%d] conf->ip=%s\n", __FUNCTION__, __LINE__,
			conf->ip);
	printf("[%s:%d] conf->port=%s\n", __FUNCTION__, __LINE__, 
			conf->port);
#endif
	fclose(fp);
}
int SocketInit(struct config *conf,bool S_or_C){
    int sockfd;

    /*设置通信结构体*/
    Addr_in addr_in;
    socklen_t len;
    len = sizeof(addr_in);
    memset((void *)&addr_in, 0,sizeof(addr_in));
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(atoi(conf->port));
    if (inet_aton(conf->ip, &addr_in.sin_addr) == 0) {
        fprintf(stderr, "Invalid address\n");
        exit(EXIT_FAILURE);
    }

    /* 创建套接字 */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
        ErrExit("socket");

    if(S_or_C){//服务端
        /*地址快速重用*/
        int b_reuse = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &b_reuse, sizeof(int) );
        /* 绑定套接字 */
        if(bind(sockfd, (Addr *)&addr_in, len))
            ErrExit("bind");
        /*设定为监听模式*/
        if(listen(sockfd, BACKLOG) < 0)
            ErrExit("listen");
    } else {//客户端

        if(connect(sockfd, (Addr *)&addr_in, len))
            ErrExit("connect");
    }
    
    return sockfd;
}
ssize_t Read(int fd, void *buf, size_t len) {
	ssize_t ret;
	do {
		ret = read(fd, buf, len);
	} while(ret < 0 && errno == EINTR);
	if(ret < 0)
		ErrExit("read");
	return ret;
}
ssize_t Write(int fd, const void *buf, size_t len) {
	ssize_t ret;
	do {
		ret = write(fd, buf, len);
	} while(ret < 0 && errno == EINTR);
	if(ret < 0)
		ErrExit("write");
	return ret;
}

