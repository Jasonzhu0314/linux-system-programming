/*************************************************************************
	> File Name: forkOpt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Tue 25 May 2021 09:51:16 AM CST
 ************************************************************************/

#include <stdio.h>
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
            printf("child[%d], pid:%d, ppid%d\n",
                   10 - n, getpid(), getppid());
            sleep(1);
            break;
        }
        --n;
    } 

    return 0;
}
