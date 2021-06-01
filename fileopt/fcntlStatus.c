/*************************************************************************
	> File Name: fcntlStatus.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Thu 20 May 2021 01:59:24 PM CST
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(int argv, char* argc[]) {
    int flags;
    if ((flags = fcntl(atoi(argc[1]), F_GETFL)) < 0) {
        perror("get file status:");
        exit(1);
    };

    if (flags != )
    




    return 0;
}
