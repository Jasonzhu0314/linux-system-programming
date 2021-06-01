/*************************************************************************
	> File Name: mmapOpt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Fri 21 May 2021 08:10:46 AM CST
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>

int main() {

    int fd = open("test.txt", O_RDWR);
    
    if (fd < 0) {
        perror("open file");
        exit(1);
    }
    
    int *p = mmap(NULL, 6, PROT_WRITE, MAP_SHARED, fd, 0);
    ((int*) (((char *) p) + 1))[0] = 0x30313233;

    munmap(p, 6);
    return 0;
}
