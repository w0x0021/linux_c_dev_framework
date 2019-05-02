/*************************************************************************
    > File Name: shm_com.h
    > Author: WangSansan
    > Mail: hackxiaowang@qq.com
    > Blog: https://www.wangsansan.com
    > Created Time: 2018年04月04日 星期三 16时39分21秒
 ************************************************************************/
 

#ifndef _SHM_H_
#define _SHM_H_

#include <sys/types.h>

int shmcom_create(key_t key, size_t size);
int shmcom_read(key_t key, void *buf, int len);
int shmcom_write(key_t key, void *data, size_t size);

#endif