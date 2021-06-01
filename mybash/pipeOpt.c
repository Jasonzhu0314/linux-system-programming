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

    if ((flag = pipe(fd)) == -1) {
        perror("pipe:");
        exit(1);
    }



    return 0;
}

