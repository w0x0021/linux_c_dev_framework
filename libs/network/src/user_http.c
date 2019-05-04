/*************************************************************************
    > File Name: user_http.c
    > Author: WangMinghang
    > Mail: hackxiaowang@qq.com
    > Blog: https://www.wangsansan.com
    > Created Time: 2018年04月12日 星期四 10时32分21秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "user_http.h"
#include "user_socket.h"


#define HTTP_DEFAULT_HEADER_LEN     4096
#define HTTP_DEFAULT_HEADER         "User-Agent: WangSansan Http 0.1\r\n"   \
                                    "Cache-Control: no-cache\r\n"           \
                                    "Content-Type: application/x-www-form-urlencoded\r\n"   \
                                    "Accept: */*\r\n"
#if 0
char *HttpDefaultHeader     =       "User-Agent: WangSansan Http 0.1\r\n"   
                                    "Cache-Control: no-cache\r\n"           
                                    "Content-Type: application/x-www-form-urlencoded\r\n"   
                                    "Accept: */*\r\n";                                    
#endif

typedef enum {
    GET,
    POST
}RequestMethod_e;

char HttpHeader[HTTP_DEFAULT_HEADER_LEN] = { 0 };


int make_header(char *host, int port, char *url, RequestMethod_e method);

static inline int make_header_get(char *host, int port, char *url);
static inline int make_header_post(char *host, int port, char *url, char *content);


int http_post(char *host, int port, char *url, int s, char *content, char **response)
{
    int len = 0;
    int sockfd = 0;
    int header_len = 0;

    // 封装HTTP头
    header_len = make_header_post(host, port, url, content);
    if(header_len <= 0){
        return -1;
    }

    // 创建TCP Client并连接到Server
    sockfd = create_connect(host, port, s);
    if(sockfd <= 0){
        return -2;
    }

    // 发送数据
    len = socket_send(sockfd, HttpHeader, strlen(HttpHeader), s);
    if(len <= 0){
        return -3;
    }

    // 读取数据
    len = socket_recv(sockfd, response, 0, s);
    if(len <= 0){
        return -4;
    }

    // 断开socket连接
    close(sockfd);

    return len;
}

int make_header(char *host, int port, char *url, RequestMethod_e method)
{
    if(host == NULL     ||
        port >= 65535   ){
            return -1;
    }

    memset(HttpHeader, 0, HTTP_DEFAULT_HEADER_LEN);
    if(method == GET){
        if(url == NULL){
            sprintf(HttpHeader, "GET / HTTP/1.0\r\n");
        }else{         
            sprintf(HttpHeader, "GET %s HTTP/1.0\r\n", url);     
        }
    }else{
        if(url == NULL){      
            sprintf(HttpHeader, "POST / HTTP/1.0\r\n");      
        }else{      
            sprintf(HttpHeader, "POST %s HTTP/1.0\r\n", url);          
        }
    }

    if(port == 0){
        sprintf(HttpHeader, "%sHost: %s:%d\r\n",HttpHeader, host, 80);  
    }else{
        sprintf(HttpHeader, "%sHost: %s:%d\r\n",HttpHeader, host, port);
    }

    //sprintf("%s%s", HttpHeader, HttpDefaultHeader);
    strcat(HttpHeader, HTTP_DEFAULT_HEADER);
    return strlen(HttpHeader);
}

static inline int make_header_get(char *host, int port, char *url)
{
    return make_header(host, port, url, GET);
}

static inline int make_header_post(char *host, int port, char *url, char *content)
{
    int len = 0;

    len = make_header(host, port, url, POST);
    if(len <= 0){
        return -1;
    }

    if(content == NULL){
        sprintf(HttpHeader, "%sContent-Length: %d\r\n\r\n", HttpHeader, 0);
        return strlen(HttpHeader);
    }

    sprintf(HttpHeader, "%sContent-Length: %d\r\n\r\n", HttpHeader, strlen(content));
    sprintf(HttpHeader, "%s%s", HttpHeader, content);
    return strlen(HttpHeader);
}

