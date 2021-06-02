/*************************************************************************
	> File Name: fileio.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Mon 17 May 2021 08:37:57 AM CST
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: cmd filename \n");
        return -1;
    } 
    
    //int fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, 0644);
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    //int fd = open(argv[1], O_WRONLY | O_CREAT,
    //                     S_IRUSR | S_IWUSR);
    
    if (fd < 0) {
        perror("Error");
        return -1;
    } else {
        perror("Open");
    }
    
    close(fd);

    return 0;
}
