/*************************************************************************
    > File Name: Network.c
    > Author: WangSansan
    > Mail: hackxiaowang@qq.com
    > Blog: https://www.wangsansan.com
    > Created Time: 2019年04月29日 星期一 10时32分21秒
 ************************************************************************/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "user_socket.h"

int create_socket();
//int create_connect(char *host, int port, int s);

int socket_timeout(int sockfd, int s);
int socket_connect(int sockfd, char* server, int port);


int create_connectx(char *host, int port, int s)
{
    int re = -1;
    int sock_fd = -1;
    unsigned long ul;

    // 创建 socket
    sock_fd = create_socket();
    if(sock_fd < 0){
        return -1;
    }

    // 设置非阻塞
    ul = 1;
    ioctl(sock_fd, FIONBIO, &ul);

    // 连接socket
    re = socket_connect(sock_fd, host, port);
    if(re == 1){
        // 设置为阻塞
        ul = 0;
        ioctl(sock_fd, FIONBIO, &ul);

        return sock_fd;
    }

    // 超时监听
    re = socket_timeout(sock_fd, s);
    if(re <= 0){
        close(sock_fd);
        return -3;
    }

    // 设置为阻塞
    ul = 0;
    ioctl(sock_fd, FIONBIO, &ul);

    return sock_fd;
}

int socket_timeout(int sockfd, int s)
{
    int re = 0;
    fd_set set;
    struct timeval tm;

    int len;
    int error = -1;

    tm.tv_sec = s;
    tm.tv_usec = 0;
    FD_ZERO(&set);
    FD_SET(sockfd, &set);

    re = select(sockfd + 1, NULL, &set, NULL, &tm);
    if(re > 0){
        len = sizeof(int);
        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
        if(error == 0){
            re = 1;
        }else{ 
            re = -3;
        }
    }else{
        re = -2;
    }

    return re;
}

int socket_connect(int sockfd, char *server, int port)
{
    int re = -1;
    struct hostent *host;
    struct sockaddr_in cliaddr;
 
    // 域名解析
    host = gethostbyname(server);
    if(host == NULL){

        printf("gethostbyname(%s) error:%s\n", server, strerror(errno));
        re = -1;
        return re;
    }

    // 填充socket的IP与端口
    bzero(&cliaddr, sizeof(struct sockaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(port);
    cliaddr.sin_addr = *((struct in_addr *)host->h_addr);
 
    // 客户端连接
    re = connect(sockfd, (struct sockaddr *)&cliaddr, sizeof(struct sockaddr));
    if(re >= 0){
        return 1;
    }

    return re;
}


int create_socket()
{
    int sockfd;
 
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        perror("create socket error");
        return(0);
    }
    return(sockfd);
}




