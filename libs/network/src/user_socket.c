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

extern int errno;

int create_socket();
int socket_connect(int sockfd, char *server, int port);

static inline int socket_timeout(int sockfd, int s, fd_set *rfds, fd_set *wfds, fd_set *efds);

int socket_send(int sockfd, char *data, int len, int s)
{
    int re = 0;
    int err = 0;
    int send = 0;
    int send_count = 0;
    int end_count = len;
    fd_set write_fds;

    while(end_count)
    {
        re = socket_timeout(sockfd, s, NULL, &write_fds, NULL);
        if(re < 0){
            err = -1;       // 超时
            break;
        }

        if(FD_ISSET(sockfd, &write_fds)){
            send = write(sockfd, data+send_count, end_count);
            if(send == 0){
                err = -2;          // 没有发送任何内容
                break;
            }

            if(send < 0){
                switch(errno)
                {
                    case EINTR:{
                        err = -3;       // 未知错误
                    }break;
                    case ENOSPC:{
                        err = -4;       // 内存空间不足
                    }break;
                    default: {
                        err = -5;       // write() 错误
                    }break;
                }
                break;
            }

            send_count  = send_count + send;
            end_count   = len - send_count;
        }
    }

    if(err < 0){
        return err;
    }

    return send_count;
}

int socket_recv(int sockfd, char **data, int size, int s)
{
    int re = 0;
    int len = 0;
    int len_count = 0;
    fd_set read_fds;

    char buffer[SOCKET_DEFAUT_MAX_SIZE];
    memset(buffer, 0, SOCKET_DEFAUT_MAX_SIZE);

    while(1)
    {
        re = socket_timeout(sockfd, s, &read_fds, NULL, NULL);
        if(re < 0){
            re = -1;            // 超时或select异常
            break;
        }

        if(FD_ISSET(sockfd, &read_fds)){
            len = read(sockfd, buffer, SOCKET_DEFAUT_MAX_SIZE);
            if(len < 0){
                re = -2;        // read异常
                break;
            }

            if(len == 0){
                break;
            }

            len_count = len_count + len;
            if(size != 0){
                if(len_count >= size){
                    break;
                }
            }
        }
    }
    
    if( re < 0              &&
        (len_count == 0)    ){
            return re;
    }

    *data = (char *)malloc(len_count);
    if(*data == NULL){
        return -3;
    }

    memset(*data, 0, len_count);
    memcpy(*data, buffer, len_count);
    return len_count;
}


int create_connect(char *host, int port, int s)
{
    int re = -1;
    fd_set fds;
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
        ul = 0;
        ioctl(sock_fd, FIONBIO, &ul);       // 设置为阻塞
        return sock_fd;
    }

    // 超时监听
    re = socket_timeout(sock_fd, s, NULL, &fds, NULL);
    if(re <= 0){
        close(sock_fd);
        return -2;
    }

    // 设置为阻塞
    ul = 0;
    ioctl(sock_fd, FIONBIO, &ul);
    return sock_fd;
}

static inline int socket_timeout(int sockfd, int s, fd_set *rfds, fd_set *wfds, fd_set *efds)
{
    int re = 0;
    struct timeval tm;

    int len;
    int error = -1;

    if(rfds != NULL){
        FD_ZERO(rfds);
        FD_SET(sockfd, rfds);
    }

    if(wfds != NULL){
        FD_ZERO(wfds);
        FD_SET(sockfd, wfds);
    }

    if(efds != NULL){
        FD_ZERO(efds);
        FD_SET(sockfd, efds);
    }

    if(s == 0){
        re = select(sockfd + 1, rfds, wfds, efds, NULL);
    }else{
        tm.tv_sec = s;
        tm.tv_usec = 0;
        re = select(sockfd + 1, rfds, wfds, efds, &tm);
    }
    
    if(re > 0){
        len = sizeof(int);
        getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
        if(error == 0){
            re = 1;         // 正常
        }else{ 
            re = -2;        // 未知错误
        }
    }else{
        if(errno != EINTR){
            re = -3;            // select() 错误
        }else{
            re = -1;            // 超时
        }
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
    if(sockfd < 0){
        //perror("create socket error");
        return -1;
    }
    return sockfd;
}
