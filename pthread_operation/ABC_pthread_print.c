/*************************************************************************
	> File Name: abc_opt.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Wed 09 Jun 2021 12:09:25 PM CST
    > Description: 三个线程轮流输出A、B、C，线程间通信 
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t print_lock = PTHREAD_MUTEX_INITIALIZER;
int flag;
int cnt = 10;

void* PrintfA(void *arg) {
    while(1) {
        pthread_mutex_lock(&print_lock);
        if (flag == 0) {
            write(STDOUT_FILENO, "A", 1);
            flag = 1;
            cnt--;
            if (cnt == 0){
                pthread_mutex_unlock(&print_lock);
                pthread_exit(NULL);
            }; 
        }
        pthread_mutex_unlock(&print_lock);
    }
}

void* PrintfB(void *arg) {
    while(1) {
        pthread_mutex_lock(&print_lock);
        if (flag == 1) {
            write(STDOUT_FILENO, "B", 1);
            flag = 2;
            if (cnt == 0){
                pthread_mutex_unlock(&print_lock);
                pthread_exit(NULL);
            }; 
        }
        pthread_mutex_unlock(&print_lock);
    }
}

void* PrintfC(void *arg) {
    while(1) {
        pthread_mutex_lock(&print_lock);
        if (flag == 2) {
            write(STDOUT_FILENO, "C", 1);
            flag = 0;
            if (cnt == 0){
                pthread_mutex_unlock(&print_lock);
                pthread_exit(NULL);
            }; 
        } 
        pthread_mutex_unlock(&print_lock);
    }
}

int main() {
    pthread_t cid1, cid2, cid3;

    pthread_create(&cid1, NULL, PrintfA, NULL);
    pthread_create(&cid2, NULL, PrintfB, NULL);
    pthread_create(&cid3, NULL, PrintfC, NULL);
    
    pthread_join(cid1, NULL);
    pthread_join(cid2, NULL);
    pthread_join(cid3, NULL);

    return 0;
}
