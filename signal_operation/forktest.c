/*************************************************************************
	> File Name: forktest.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Thu 03 Jun 2021 08:30:32 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t cpid;

void my_handler() {
    printf("cpid: %d\n", cpid);
    kill(cpid, SIGUSR2);
    // if ( kill(cpid, SIGUSR2) == 0 ){
    //     printf("send sucess\n");
    // };
    // int sts;

    // pid_t cpid1 = wait(&sts);
    // if (WIFEXITED(sts)) {
    //     //获取儿子的退出状态，返回状态码
    //     printf("proc: %d exit with code %d\n", cpid1, WEXITSTATUS(sts));
    // } else 
    //     printf("Lemon\n");
    wait(0);
    exit(0);
    return ;
}

void Sigusr1Handler(int signo) {
    printf("Sigusr1Handler\n");
    exit(0) ; 
}

void SigintHandler(int signo) {
    //printf("SigintHandler\n");
    //exit(0) ; 
    return ;
}

int main() {
    // signal(SIGUSR2, Sigusr1Handler);
    // signal(SIGINT, my_handler);
    // struct sigaction act1, old_act1;
    // act1.sa_flags = 0;
    // act1.sa_handler = my_handler;
    // sigemptyset(&act1.sa_mask);
    // //sigaddset(&act.sa_mask, SIGINT);
    // sigaction(SIGINT, &act1, &old_act1);


    // struct sigaction act, old_act;
    // act.sa_flags = 0;
    // act.sa_handler = Sigusr1Handler;
    // sigemptyset(&act.sa_mask);
    // sigaddset(&act.sa_mask, SIGUSR2);
    // sigaction(SIGUSR2, &act, &old_act);
    signal(SIGINT, my_handler);

    cpid = fork();

    if (cpid) {
        printf("son pid:%d\n", cpid);
        // struct sigaction act1, old_act1;
        // act1.sa_flags = 0;
        // act1.sa_handler = my_handler;
        // sigemptyset(&act1.sa_mask);
        // //sigaddset(&act.sa_mask, SIGINT);

        //sigaction(SIGUSR1, &act1, &old_act1);

        //signal(SIGUSR1, my_handler);
        signal(SIGINT, my_handler);
        signal(SIGUSR1, my_handler);
        //kill(cpid, SIGUSR2);
        //pause();
        //wait(0);
        while(1);
    } else {
        // struct sigaction act, old_act;
        // act.sa_flags = 0;
        // act.sa_handler = Sigusr1Handler;
        // sigemptyset(&act.sa_mask);
        // sigaddset(&act.sa_mask, SIGUSR2);
        // sigaction(SIGUSR2, &act, &old_act);
        signal(SIGINT, SigintHandler);
        signal(SIGUSR2, Sigusr1Handler);

        //printf("endi\n");

        while(1) {
            printf("endi\n");
            sleep(1);
        };

        // if (pause() == -1) {
        //     perror("pause:");
        //     exit(1);
        // };
        printf("endi\n");
    }
    return 0;
}
