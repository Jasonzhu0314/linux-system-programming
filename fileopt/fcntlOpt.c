/*************************************************************************
	> File Name: fcntlOpt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Thu 20 May 2021 08:12:44 AM CST
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main() {
    int flags;
    if ((flags = fcntl(STDIN_FILENO, F_GETFL) < 0)) {
        perror("fcntl get erroe");
        return 1;
    }
    flags |= O_NONBLOCK;
    if ((flags = fcntl(STDIN_FILENO, F_SETFL, flags) < 0)) {
        perror("fcntl set error");
    }

    char buf[10];
    ssize_t n;
    while (1) {
        n = read(0, buf, 5);
        if (n >= 0)
            break;
        if (errno != EAGAIN) {
            perror("read");
            return -1;
        }
        write(1, "try\n", 4);
        sleep(1);
    }

    write(1, buf, n);






    return 0;
}
