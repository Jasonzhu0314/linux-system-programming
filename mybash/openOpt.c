/*************************************************************************
	> File Name: openOpt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Sat 29 May 2021 07:21:46 AM CST
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>


int main() {
    
    char buff[] = "ab";

    int fd;
    if (~(fd = open(buff, O_WRONLY|O_CREAT|O_TRUNC, 0664))) {
        perror("open:");
        exit(1);
    };
    printf("fd:%d\n", fd);
    

    return 0;
}

