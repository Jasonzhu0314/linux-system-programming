/*************************************************************************
	> File Name: statOpt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Sat 22 May 2021 09:45:18 AM CST
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    struct stat st;
    
    lstat(argv[1], &st);
    switch(st.st_mode & S_IFMT) {
        case S_IFREG:
            printf("regular file\n");
            break;
        case S_IFDIR:
            printf("directory \n");
            break;
        case S_IFCHR:
            printf("charactor device\n");
            break;
        default:
            printf("other file type\n");
            break;
    }
    

    return 0;

}
