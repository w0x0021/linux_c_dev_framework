/*************************************************************************
    > File Name: shm_com.c
    > Author: WangSansan
    > Mail: hackxiaowang@qq.com
    > Blog: https://www.wangsansan.com
    > Created Time: 2018年04月04日 星期三 16时39分21秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

#include "shm_com.h"
#include "semaphore.h"

#define SHM_KEY_START 		0x21
#define SHM_KEY_CALC(k)		SHM_KEY_START+k
#define SEM_KEY_CALC(k)		SHM_KEY_CALC(k)*3

int shmcom_create(key_t key, size_t size)
{
	/*** Shm Open ***/
	int shmid;
	shmid 	= shmget(SHM_KEY_CALC(key), size, 0666|IPC_CREAT);
	if(shmid == -1){
		return -1;
	}

	/*** Shm Active ***/
	char *mem 	= shmat(shmid, 0, 0);
	if(mem == (void *)-1){
		return -1;
	}

	/*** Close ***/
	shmdt(mem);

	return size;
}

int shmcom_write(key_t key, void *data, size_t size)
{
	if(data == NULL){
		return -1;
	}

	/*** Shm Open ***/
	int shmid;
	shmid 	= shmget(SHM_KEY_CALC(key), size, 0666|IPC_CREAT);
	if(shmid == -1){
		return -1;
	}

	/*** Shm Active ***/
	char *mem 	= shmat(shmid, 0, 0);
	if(mem == (void *)-1){
		return -1;
	}

	/*** Sem Open ***/
	int semid;
	semid 	= semget(SEM_KEY_CALC(key), 1, 0666|IPC_CREAT);
	if(semid == -1){
		return -1;
	}
	/*** Sem Init ***/
	set_semvalue(semid, 1);

	/*** Operation ***/
	semaphore_p(semid);
	memcpy(mem, data, size);

	semaphore_v(semid);

	/*** Close ***/
	shmdt(mem);
	del_semvalue(semid);

	return size;
}

int shmcom_read(key_t key, void *buf, int len)
{
	if(buf == NULL){
		return -1;
	}

	/*** Open ***/
	int shmid;
	shmid 	= shmget(SHM_KEY_CALC(key), len, 0440|IPC_CREAT);
	if(shmid == -1){
		return -1;
	}

	/*** Active ***/
	char *mem 	= shmat(shmid, 0, 0);
	if(mem == (void *)-1){
		return -1;
	}

	/*** Sem Open ***/
	int semid;
	semid 	= semget(SEM_KEY_CALC(key), 1, 0666|IPC_CREAT);
	if(semid == -1){
		return -1;
	}
	/*** Sem Init ***/
	set_semvalue(semid, 1);

	/*** Operation ***/
	semaphore_p(semid);
	memcpy(buf, mem, len);

	semaphore_v(semid);

	/*** Close Shm&Sem***/
	shmdt(mem);
	del_semvalue(semid);

	return len;
}