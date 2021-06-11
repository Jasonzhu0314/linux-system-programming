/*************************************************************************
	> File Name: execOpt.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Sat 05 Jun 2021 08:26:33 AM CST
    > decription: 测试exec函数族中的各个参数的区别和修改envp环境变量测试
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[], char *envp[]) {
    char *arg1[] = {"ls", NULL};

    //printf("execl:\n");
    //execl("/bin/ls", "ls", NULL);

    // argv数组
    //printf("execv:\n");
    //execv("/bin/ls", argv);

    //printf("execlp:\n");
    //execlp("ls", "ls", NULL);
    //printf("execvp:\n");
    //execvp("ls", argv);
    
    // 测试修改envp中的PATH，对exec 的 p系列的影响，使用putenv和getenv函数
    char **temp = envp;
    
    printf("PATH=%s\n", getenv("PATH"));
    putenv("PATH=."); //临时修改PATH
    printf("PATH=%s\n", getenv("PATH"));

    printf("execlp:\n");
    execlp("ls", "ls", NULL); // 这行代码不会执行，因为ls不在环境变量中
    putenv("PATH=/bin"); // ls在/bin目录的可执行文件下
    
    printf("execlp add bin:\n");
    execlp("ls", "ls", NULL); // 可以执行

    printf("execle:\n");
    execle("/bin/ls", "ls", NULL, envp);
    printf("execve:\n");
    execve("/bin/ls", arg1, envp);

    return 0;
}
