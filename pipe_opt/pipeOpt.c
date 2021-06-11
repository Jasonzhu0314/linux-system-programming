/*************************************************************************
	> File Name: pipeOpt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Sun 30 May 2021 08:03:35 AM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int fd[2];
    int flag;
    pid_t cpid;
    char buf[20];

    if ((flag = pipe(fd)) == -1) {
        perror("pipe:");
        exit(1);
    }

    if ((cpid = fork()) < 0) {
        perror("fork:");
        exit(1);
    } 

    if (cpid) {
        close(fd[0]);
        write(1, "writing\n", 8);
        sleep(2);
        write(fd[1],"I am write\n", 11);
        
    } else {
        close(fd[1]);
        int n;
        if ( (n = read(fd[0], buf, 20)) < 0 ) {
            perror("read pipe");
            exit(1);
        };
        printf("read num:%d\n", n);
        write(STDOUT_FILENO, buf, n);
    }

    return 0;
}

