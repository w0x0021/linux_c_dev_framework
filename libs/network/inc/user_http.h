/*************************************************************************
    > File Name: user_http.h
    > Author: WangMinghang
    > Mail: hackxiaowang@qq.com
    > Blog: https://www.wangsansan.com
    > Created Time: 2018年04月12日 星期四 10时32分21秒
 ************************************************************************/
 
#ifndef __USER_HTTP_H__
#define __USER_HTTP_H__

/**
 * @Name                http_post
 * @Description         HTTP的POST提交
 * @Parameter       -----
 *                  *host       主机名, 域名或者IP地址
 *                   port       远程端口地址
 *                  *url        URL链接地址
 *                   s          连接、发送、接收的超时时间, 0为默认
 *                  *content    需要POST的内容
 *                  **response  响应数据包的存放地址, 
 *                                  该参数会根据接收到的数据申请内存, 需要在使用完成后释放
 *
 * @Return          (int)
 *                  成功      -   返回获取到的数据包大小
 *                  失败      -
 *                                -1    HTTP头构造失败
 *                                -2    socket连接失败
 *                                -3    socket发送数据失败
 *                                -4    socket接收数据失败
 */
int http_post(char *host, int port, char *url, 
                int s, char *content, char **response);

#endif
