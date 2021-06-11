/*************************************************************************
	> File Name: processSignal.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Thu 03 Jun 2021 05:39:28 PM CST
    > Document: 父进程和子进程之间的通信
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t cpid[2];

pid_t Fork() {
    // 将fork函数封装
    pid_t cpid;
    cpid = fork();
    if (cpid < 0) {
        perror("fork");
        exit(1);
    }
    return cpid;
}

void FatherSigintHandler() {
    // 父进程的SIGINT信号处理函数，向子进程发送信号
    kill(cpid[0], SIGUSR1);
    kill(cpid[1], SIGUSR2);
    // 等待收尸
    wait(0);
    exit(0);
}

void Sigusr1Handler() {
    // 子进程的SIGUSR1信号处理函数
    printf("child1 is killed by parent\n");
    exit(0);
}

void Sigusr2Handler() {
    // 子进程的SIGUSR2信号处理函数
    printf("child2 is killed by parent\n");
    exit(0);
}

void SonSigintHandler() {
    // SIGINT信号同样会传递给子进程，所以子进程的SIGINT信号处理函数要忽略信号
    return;
}

void Sigaction(int num, void  (*func)()) {
    // 先阻塞信号，再修改动作
    sigset_t new_mask, old_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask, num);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);

    // 修改信号处理动作
    struct sigaction act, old_act;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, num);
    act.sa_flags = 0;
    act.sa_handler = func;
    sigaction(num, &act, &old_act);

    // 恢复信号
    sigprocmask(SIG_SETMASK, &old_mask, NULL);
}

int main() {

    cpid[0] = Fork();
    
    if (cpid[0]) {
        cpid[1] = Fork();
        if (cpid[1]) {
            // 父进程绑定自定义SIGINT信号处理函数
            Sigaction(SIGINT, FatherSigintHandler);
            while(1);
        } else {
            // 子进程绑定自定义SIGINT和SIGUSR2信号处理函数
            Sigaction(SIGINT, SonSigintHandler);
            Sigaction(SIGUSR2, Sigusr2Handler);
            while(1);
        }
    } else {
        // 子进程绑定自定义SIGINT和SIGUSR1信号处理
        Sigaction(SIGINT, SonSigintHandler);
        Sigaction(SIGUSR1, Sigusr1Handler);
        while(1);
    }

    return 0;
}
