/*************************************************************************
	> File Name: child.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Wed 02 Jun 2021 08:20:31 PM CST
    > document: 使用fork测试SIGCHLD信号，父进程非阻塞给子进程收尸
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void show_handler() {
    int sts;
    //等待给儿子收尸
    pid_t cpid = wait(&sts);
    if (WIFEXITED(sts)) {
        //获取儿子的退出状态，返回状态码
        printf("proc: %d exit with code %d\n", cpid, WEXITSTATUS(sts));
    } else 
        printf("Lemon\n");

    return;
}

int main() {
    pid_t cpid = fork(); 
    if (cpid < 0) {
        perror("fork:");
        exit(1);
    }

    if (cpid) {
        printf("create child pid:%d\n", cpid);
        cpid = fork();
        if (cpid) {
            printf("create child pid:%d\n", cpid);
            //定义收到SIGCHLD信号后的动作，调用wait
            struct sigaction act, oldact;
            act.sa_handler = show_handler;
            sigemptyset(&act.sa_mask);
            sigaddset(&act.sa_mask, SIGCHLD);
            act.sa_flags = 0;
            sigaction(SIGCHLD, &act, &oldact);

            int n = 10;
            while(n--) {
                sleep(1);
                printf("father sleep 1~\n");
            }
        } else {
            sleep(5);
            exit(5);
        } 
    } else {
        sleep(3);
        exit(2);
    }

    return 0;
}
