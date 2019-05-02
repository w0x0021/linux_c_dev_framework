#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <errno.h>
#include <pthread.h>

#define MAX_WAITING_TASKS	1000
#define MAX_ACTIVE_THREADS	100

struct task
{
	void *(*do_task)(void *arg);		// 线程任务回调
	void *arg; 							// 传递到回调函数的参数指针

	struct task *next; 					// 下一个任务指针
};

typedef struct thread_pool
{
	pthread_mutex_t lock;				// 互斥锁
	pthread_cond_t  cond; 				// 条件变量

	bool shutdown;

	struct task *task_list;				// 线程任务单链

	pthread_t *tids;					// 线程pid

	unsigned max_waiting_tasks;			// 最大等待任务数量
	unsigned waiting_tasks; 			// 等待任务数量
	unsigned active_threads; 			// 活跃任务数量
}thread_pool;

bool init_pool(thread_pool *pool, unsigned int threads_number);
bool add_task(thread_pool *pool, void *(*do_task)(void *arg), void *task);
int  add_thread(thread_pool *pool, unsigned int additional_threads_number);
int  remove_thread(thread_pool *pool, unsigned int removing_threads_number);
bool destroy_pool(thread_pool *pool);

void *routine(void *arg);


#endif
