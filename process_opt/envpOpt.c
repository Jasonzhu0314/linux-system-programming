/*************************************************************************
	> File Name: envpOpt.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Mon 07 Jun 2021 09:12:43 AM CST
 ************************************************************************/

#include <stdio.h>

int main(int argc, char* argv[], char *envp[]) {
    while(*envp) printf("%s\n", *(envp++));

    return 0;
}
