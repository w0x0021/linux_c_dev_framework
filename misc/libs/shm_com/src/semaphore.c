/*
 * File
 * semaphore.c
 * Breif
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

#include "semaphore.h"

/* init semaphore by semctl */
int set_semvalue(int sem_id, int value)
{
    union semun sem_union;
      
    sem_union.val = value;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
      return 1;

    return 0;
}


/* delete semaphore by sectl */
void del_semvalue(int sem_id)
{
    union semun sem_union;

    if(semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
      fprintf(stderr, "Failed to delete semaphore\n");
}

/* P(v) */
int semaphore_p(int sem_id)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; /* P(v) */
    //sem_b.sem_flg = IPC_NOWAIT;
    sem_b.sem_flg = SEM_UNDO;

    if (semop(sem_id, &sem_b, 1) == -1)
    {
      fprintf(stderr, "semaphore_p failed\n");
      return 1;
    }

    return 0;
}

/* V(v) */
int semaphore_v(int sem_id)
{
    struct sembuf sem_b;

    sem_b.sem_num = 0;
    sem_b.sem_op = 1; // V(v)
    //sem_b.sem_flg = IPC_NOWAIT;
    sem_b.sem_flg = SEM_UNDO;

    if (semop(sem_id, &sem_b, 1) == -1)
    {
      fprintf(stderr, "semaphore_v failed\n");
      return 1;
    }

    return 0;
}