/*************************************************************************
	> File Name: semOpt.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Wed 09 Jun 2021 07:49:07 AM CST
    > Description: 使用信号量，多个消费者，多个生产者，线程间通信
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM 5

sem_t g_sem_producer;
sem_t g_sem_consumer;
pthread_mutex_t head_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tail_lock = PTHREAD_MUTEX_INITIALIZER;

int g_queue[NUM];
int g_head, g_tail;

void *Producer(void *arg) {
    int temp;
    while(1) {
        sem_wait(&g_sem_producer);
        temp = rand() % 100 + 1;
        {
            pthread_mutex_lock(&tail_lock);
            g_queue[g_tail] = temp;
            printf("id:%lu, put number: %d\n",pthread_self(), g_queue[g_tail]);
            g_tail = (g_tail + 1) % NUM;
            pthread_mutex_unlock(&tail_lock);
        }
        sem_post(&g_sem_consumer);
        sleep(rand() % 2);
    }
}

void *Consumer(void *arg) {
    while(1) {
        sem_wait(&g_sem_consumer);
        {
            pthread_mutex_lock(&head_lock);
            printf("id:%lu, get number: %d\n", pthread_self(), g_queue[g_head]);
            g_head = (g_head + 1) % NUM;
            pthread_mutex_unlock(&head_lock);
        }
        sem_post(&g_sem_producer);
        sleep(rand() % 2);
    }
}

int main() {
    pthread_t pid1, pid2, cid1, cid2;

    sem_init(&g_sem_producer, 0, NUM);
    sem_init(&g_sem_consumer, 0, 0);

    pthread_create(&pid1, NULL, Producer, NULL);
    pthread_create(&pid2, NULL, Producer, NULL);
    pthread_create(&cid1, NULL, Consumer, NULL);
    pthread_create(&cid2, NULL, Consumer, NULL);

    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    pthread_join(cid1, NULL);
    pthread_join(cid2, NULL);

    return 0;
}
