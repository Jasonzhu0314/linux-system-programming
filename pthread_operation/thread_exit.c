/*************************************************************************
	> File Name: thread_exit.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Sun 06 Jun 2021 08:50:33 AM CST
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *thr_fn1(void *arg) {
    printf("thread 1 returning \n");
    return (void*) 1;
}

void *thr_fn2(void *arg) {
    printf("thread 2 exit \n");
    pthread_exit((void *) 2);
    return NULL;
}

void *thr_fn3(void *arg) {
    printf("thread 3 returning \n");
    while(1) {
        printf("thread 3 sleeping\n");
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t tid;
    void *sts;
    pthread_create(&tid, NULL, thr_fn1, NULL);
    pthread_join(tid, &sts);
    printf("thread1 return %p\n", sts);
    
    pthread_create(&tid, NULL, thr_fn2, NULL);
    pthread_join(tid, &sts);
    printf("thread2 return %p\n", sts);

    sleep(3);
    pthread_create(&tid, NULL, thr_fn1, NULL);
    pthread_cancel(tid);
    pthread_join(tid, &sts);
    printf("thread3 return %p\n", sts);
    
    return 0;
}
