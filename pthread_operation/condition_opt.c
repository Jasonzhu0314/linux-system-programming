/*************************************************************************
	> File Name: ConditionOpt.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Tue 08 Jun 2021 12:38:30 PM CST
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct Goods {
    int data;
    struct Goods *next;
} Goods;

Goods* head = NULL;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t produce_cond = PTHREAD_COND_INITIALIZER;

void* Producer(void *arg) {
    Goods* temp;
    while (1) {
        temp = (Goods *) malloc(sizeof(Goods));
        temp->data = rand() % 10;
        pthread_mutex_lock(&lock);
        temp->next = head;
        head = temp;
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&produce_cond);
        printf("producer: %d\n", temp->data); 
        sleep(rand() % 3);
    }
}

void* Consumer(void *arg) {
    Goods *temp;
    while (1) {
        pthread_mutex_lock(&lock);
        while (!head) {
            pthread_cond_wait(&produce_cond, &lock);
        }
        temp = head;
        head = temp->next; 
        pthread_mutex_unlock(&lock);
        printf("consumer: %d\n", temp->data); 
        free(temp);
        sleep(rand() % 3);
    }
}

int main() {
    pthread_t pid, cid, cid1, cid2;
    pthread_create(&pid, NULL, Producer, NULL);
    pthread_create(&cid1, NULL, Consumer, NULL);
    pthread_create(&cid2, NULL, Consumer, NULL);
    

    pthread_join(pid, NULL);
    pthread_join(cid, NULL);
    pthread_join(cid1, NULL);
    pthread_join(cid2, NULL);

    return 0;
}
