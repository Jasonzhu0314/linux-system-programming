/*************************************************************************
	> File Name: ioctlOpt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Thu 20 May 2021 10:12:07 PM CST
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>


int main() {
    struct winsize size;
    
    if (ioctl(1, TIOCGWINSZ, &size) < 0) {
        perror("iotcl");
        exit(1);
    }
    
    printf("%d row, %d columns\n", size.ws_row, size.ws_col);

    return 0;
}

