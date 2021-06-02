/*************************************************************************
	> File Name: execOpt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Fri 28 May 2021 08:53:31 AM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int fd = open("./test.txt", O_WRONLY | O_CREAT, 0664);
    dup2(fd, 1);
    char* argv[] = {"ls", "-l", NULL};
    //execlp("ls", "ls","-l\n", NULL);
    execvp(argv[0], argv);


    return 0;
}
