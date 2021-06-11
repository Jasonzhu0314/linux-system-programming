/*************************************************************************
	> File Name: waitOpt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Wed 26 May 2021 08:04:27 AM CST
    > description: 获取并打印子进程的状态
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {

    pid_t cpid = fork();
    if (cpid < 0) {
        perror("fork:");
        exit(1);
    }
    if (cpid == 0) {
        int n = 5;
        while (n > 0) {
            printf("child process[%d]\n", n);
            sleep(1);
            n--;
        }
        exit(5);
    } else {
        int stat_val;
        waitpid(cpid, &stat_val, 0);
        if (WIFEXITED(stat_val)) {
            printf("Child exeited with code %d\n", WEXITSTATUS(stat_val));
        } else if (WIFSIGNALED(stat_val)) {
            printf("Child terminatd abnormally, signal %d\n", WTERMSIG(stat_val));
        }
    }

    return 0;
}
