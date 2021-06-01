/*************************************************************************
	> File Name: dupOpt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Tue 25 May 2021 09:38:06 AM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main() {

    int fd, save_fd;
    if ((fd = open("./test.txt", O_RDWR)) < 0) {
        perror("Open");
        return 1;
    }

    dup2(1, save_fd);
    dup2(fd, 1);
    write(1, "12345\n", 6);
    close(fd);
    dup2(save_fd, 1);
    write(1, "123456\n", 7);


    return 0;
}
