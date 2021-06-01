/*************************************************************************
> File Name: myBashOpt.c
> Author:Jasonzhu 
> Mail: 
> Created Time: Fri 28 May 2021 07:41:09 AM CST
************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


int ParseArgv(const char* command, char* argv[], char* director) {
    int i = 0;
    int j = 0;
    int k = 0;
    int flag = 0;
    
    while(command[i] != '>' && command[i] != '<' && command[i] && command[i] != '\n')  {
        while(command[i] == ' ') i++;
        if (command[i] == '\n' || command[i] == '>' || command[i] == '<') break; 
        j = 0;
        char* temp = (char *) malloc(sizeof(char) * 10); 
        if (command[i]) {
            while(command[i] != ' ' && command[i] && command[i] != '\n') temp[j++] = command[i++];
            argv[k++] = temp;
        }
    }

    if (!command[i] || command[i] == '\n')  {
        argv[k] = NULL;
        return 0;
    }

    if (command[i] == '>') flag = 1;
    if (command[i] == '<') flag = 2;

    do {
        i++;
    } while (command[i] == ' ');
    
    j = 0;
    while(command[i] != ' ' && command[i] != '\n') director[j++] = command[i++];
    argv[k] = NULL;

    return flag;
}

void FileDirect(const char* director,int flag) {
    int fd;
    if (flag == 1) {
        if ((fd = open(director, O_WRONLY|O_CREAT|O_TRUNC, 0664)) < 0) {
            perror("open out:");
            exit(1);
        } 
        
        if ((dup2(fd, 1)) < 0) {
            perror("dup2_out:");
            exit(1);
        }
    } else {
        if ((fd = open(director, O_RDONLY)) < 0) {
            perror("open in:");
            exit(1);
        } 

        if ((dup2(fd, 0)) < 0) {
            perror("dup2_in:");
            exit(1);
        }
    }
}

void Process(const char *command) {
    char director[100] = {0};
    char* argv[10];
    int flag;

    if ((flag = ParseArgv(command, argv, director)) != 0) {
        FileDirect(director,flag);
    }
    
    if ((flag = execvp(argv[0], argv)) < 0){
        perror("execlp:");
        exit(1);
    }
    int i = 0;
    
    while (argv[i] != NULL) {
        free(argv[i]);
        i++;
    }
}

void MultiCommand(const char* command1, const char* command2) {
    int pipe_fd[2];
    pid_t cpid;
    if (pipe(pipe_fd) < 0) {
        perror("pipe:");
        exit(1);
    }
    if ((cpid = fork()) < 0) {
        perror("fork:");
        exit(1);
    }
    if (cpid == 0) {
        // son
        close(pipe_fd[0]);
        dup2(pipe_fd[1], 1);
        Process(command1);
    } else {
        // father
        close(pipe_fd[1]);
        dup2(pipe_fd[0], 0);
        Process(command2);
    }
}

void ParseCommand(const char* buf, ssize_t n) {
    char command1[100] = {0};
    char command2[100] = {0};
    int i = 0;
    int j = 0;

    while (buf[i] == ' ')  i++;
    while(buf[i] && buf[i] != '|') command1[j++] = buf[i++];

    if (buf[i] == '|') {
        i++;
        j = 0;
        while(buf[i] == ' ') i++;
        while(buf[i]) command2[j++] = buf[i++];
        MultiCommand(command1, command2);
    } else {
        Process(command1);
    }
}

int main() {

    write(1, "input command:", 14);
    char buf[100] = {0};
    ssize_t n;
    while(~(n = read(0, buf, 100))) {
        pid_t cpid;
        if ((cpid = fork()) == 0) {
            ParseCommand(buf, n);
        } else{
            wait(0);
            write(1, "input command:", 14);
        }
    }

    return 0;
}

