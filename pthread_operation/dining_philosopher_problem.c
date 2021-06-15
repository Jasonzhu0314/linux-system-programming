/*************************************************************************
	> File Name: dining_philosopher_problem.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Thu 10 Jun 2021 09:06:55 AM CST
 ************************************************************************/

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

sem_t g_sem_producer[5];
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

void Eat(int num) {
    sleep(rand() % 2);
    printf("pthread id %lu, %d eating done\n",pthread_self(), num);
    return ;
}

void Think(int num) {
    sleep(rand() % 2);
    //printf("pthread id %lu, %d eating done\n",pthread_self(), num);
    return ;
}

void* func(void *arg) {
    int philosopher_no = *((int *) arg); 
    
    while(1) {
        //pthread_mutex_lock(&g_mutex);
        Think(philosopher_no);
        sem_wait(&g_sem_producer[philosopher_no]);
        sem_wait(&g_sem_producer[(philosopher_no + 1) % 5]);
        //pthread_mutex_unlock(&g_mutex);
        Eat(philosopher_no);
        sem_post(&g_sem_producer[philosopher_no]);
        sem_post(&g_sem_producer[(philosopher_no + 1) % 5]);
        //break;
    }
}


int main() {
    pthread_t tid[5];
    int i_array[5];

    int i;
    for (i = 0; i < 5; i++) {
        sem_init(&g_sem_producer[i], 0, 1);
    }

    for (i = 0; i < 5; i++) {
        i_array[i] = i;
        pthread_create(tid + i, NULL, func, (void *) &i_array[i]);
    }
    
    for (i = 0; i < 5; i++) {
        pthread_join(tid[i], NULL);
    }
    return 0;
}
