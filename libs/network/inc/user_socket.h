/*************************************************************************
    > File Name: Network.h
    > Author: WangSansan
    > Mail: hackxiaowang@qq.com
    > Blog: https://www.wangsansan.com
    > Created Time: 2019年04月29日 星期一 10时32分21秒
 ************************************************************************/


#ifndef __USER_SOCKET_H__
#define __USER_SOCKET_H__



#define SOCKET_DEFAUT_MAX_SIZE 10240


/**
 * @Name                create_connect
 * @Description         创建一个socket连接的客户端
 * @Parameter       -----
 *                  *host       主机名, 域名或者IP地址
 *                   port       远程端口地址
 *                   s          超时时间, 0为默认
 *
 * @Return          (int)
 *                  成功      -   返回可操作性的socket, 后续使用该socket进行数据收发
 *                  失败      -
 *                                -1    socket创建失败
 *                                -2    超时或连接失败
 */
int create_connect(char *host, int port, int s);

/**
 * @Name                socket_send
 * @Description         socket发送数据
 * @Parameter       -----
 *                  sockfd      socket标识符
 *                  *data       要发送的数据内容
 *                  len         要发送的数据内容的数据长度
 *                  s           超时时间, 0为默认
 *
 * @Return          (int)
 *                  成功      -   返回获取到的数据包大小
 *                  失败      -
 *                                -1    超时
 *                                -2    没有发送任何内容
 *                                -3    未知错误
 *                                -4    内存空间不足
 *                                -5    write() 错误
 */
int socket_send(int sockfd, char *data, int len, int s);

/**
 * @Name                socket_recv
 * @Description         socket接收数据
 * @Parameter       -----
 *                  sockfd      socket标识符
 *                  **data      要存放数据的指针地址, 该函数会申请内存, 需要在使用完成之后释放
 *                  size        是否需要指定接收参数的大小, 如果不指定则令该参数为0
 *                  s           超时时间, 0为默认
 *
 * @Return          (int)
 *                  成功      -   返回获取到的数据包大小
 *                  失败      -
 *                                -1    超时或select()异常
 *                                -2    read() 异常
 *                                -3    申请内存出错
 */
int socket_recv(int sockfd, char **data, int size, int s);

#endif
