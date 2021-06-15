/*************************************************************************
	> File Name: forkOpt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Tue 25 May 2021 09:51:16 AM CST
    > description: 创建十个子进程测试, 防止变成孤儿进程，使用wait收尸
 ************************************************************************/

#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

    int n = 10;
    while (n) {
        pid_t cpid = fork();
        if (cpid < 0) {
            perror("create child");
            return(1);
        }
        if (!cpid) {
            // 子进程打印完之后要跳出循环，return函数，不然的话，会回到18行，创建孙子进程
            printf("child[%d], pid:%d, ppid%d\n",
                   10 - n, getpid(), getppid());
            break;
        }
        --n;
        // 要等待给子进程收尸，如果不等待的话，父进程先死，子进程变成孤儿进程会被init父进程接管
        wait(NULL); // 进程阻塞，等待子进程结束
    } 

    return 0;
}
