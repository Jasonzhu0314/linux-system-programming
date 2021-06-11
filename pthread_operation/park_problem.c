/*************************************************************************
    > File Name: park_problem.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Fri 11 Jun 2021 09:13:29 AM CST
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

int car_num = 0;
sem_t car_signal[2];

pthread_cond_t car_in_signal = PTHREAD_COND_INITIALIZER;
pthread_cond_t car_out_signal = PTHREAD_COND_INITIALIZER;
pthread_mutex_t g_exit_lock = PTHREAD_MUTEX_INITIALIZER;

void* ParkingIn(void *arg) {
    while (1) {
        sem_wait(car_signal);
        pthread_mutex_lock(&g_exit_lock);
        printf("a car in parking, current car_num:%d\n", car_num);
        pthread_mutex_unlock(&g_exit_lock);
    }
}

void* ParkingOut(void *arg) {
    while (1) {
        sem_wait(car_signal + 1);
        pthread_mutex_lock(&g_exit_lock);
        printf("a car out parking, current car_num:%d\n", car_num);
        pthread_mutex_unlock(&g_exit_lock);
        pthread_mutex_unlock(&g_exit_lock);
    }
}

int main() {
    pthread_t cid1, cid2;

    sem_init(car_signal, 0, 0);
    sem_init(car_signal + 1, 0, 0);
    
    pthread_create(&cid1, NULL, ParkingIn, NULL);
    pthread_create(&cid2, NULL, ParkingOut, NULL);

    int cur;

    while(1) {
        
        cur = rand() % 2;
        sleep(cur);
        pthread_mutex_lock(&g_exit_lock); 
        if (cur == 1) {
            // 入库
            if (car_num < 3) {
                sem_post(car_signal);
                car_num++; 
            } else {
                // 库内没车位
                printf("no carport, please wait space, current car num %d\n", car_num);
            }
        } else {
            // 出库
            if (car_num > 0) {
                sem_post(car_signal + 1);
                car_num--;
            } else {
                // 库内没车
                printf("no car, please wait in, current car num %d\n", car_num);
            }
        }
        pthread_mutex_unlock(&g_exit_lock); 
    }

    return 0;
}
