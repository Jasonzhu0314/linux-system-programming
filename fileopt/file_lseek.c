/*************************************************************************
	> File Name: file_lseek.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Thu 20 May 2021 07:39:03 AM CST
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>


int main() {
    int fd;
    if ((fd = open("./hello.txt", O_RDONLY)) < 0) {
        perror("OPEN");
        //exit(1);
        return 0;
    }

    char c;
    read(fd, &c, 1);
    write(STDOUT_FILENO, &c, 1);

    lseek(fd, 3, SEEK_SET);
    read(fd, &c, 1);
    write(STDOUT_FILENO, &c, 1);

    return 0;
}
