/*************************************************************************
    > File Name: http_test.c
    > Author: WangMinghang
    > Mail: hackxiaowang@qq.com
    > Blog: https://www.wangsansan.com
    > Created Time: 2018年05月04日 星期六 18时32分21秒
 ************************************************************************/


#include <stdio.h>
#include <stdlib.h>

#include "user_http.h"

int main(int argc, char **argv)
{
    int len = 0;
    char *response;

    len = http_post("lisin.wangsansan.com", 80, "/posttest.php", 10, "{\"a\":\"1\", \"b\":\"2\"}", &response);
    //len = http_post("lisin.wangsansan.com", 80, "/posttest.php", 10, "a=1&b=3", &response);
    if( len <= 0            ||
        response == NULL    ){
            printf("code = %d \n", len);
            return 0;
    }

    printf("[ Recv length - %d ]\n", len);
    printf("%s\n", response);
    free(response);

    return 0;
}
