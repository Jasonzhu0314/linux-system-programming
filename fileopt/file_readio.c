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
    
    int fd = open(argv[1], O_RDONLY);
    
    if (fd < 0) {
        perror("Error");
        return -1;
    } else {
        printf("%d\n", fd);
    }

    char buf[20];
    ssize_t n;
    int i;

    n = read(fd, buf, 10);
    for (i = 0; i < n; i++) {
        printf("%c", buf[i]);
    }
    printf("\n");

    
    
    close(fd);

    return 0;
}
