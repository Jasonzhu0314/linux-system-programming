/*************************************************************************
	> File Name: pipeOpt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Sun 30 May 2021 08:03:35 AM CST
    > Description: 父进程创建子进程，跟子进程进行通信
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int fd[2];
    int flag, n;
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
        // 父进程关闭读端
        close(fd[0]);
        write(1, "writing\n", 8);
        //从标准输入中读取数据
        n = read(STDIN_FILENO, buf, 20);
        write(fd[1], buf, n);
    } else {
        // 子进程关闭写端
        close(fd[1]);
        if ( (n = read(fd[0], buf, 20)) < 0 ) {
            perror("read pipe");
            exit(1);
        };
        printf("read num:%d\n", n);
        write(STDOUT_FILENO, buf, n);
    }

    return 0;
}

