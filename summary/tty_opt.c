/*************************************************************************
	> File Name: tty_opt.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Wed 09 Jun 2021 08:56:50 PM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

int main() {

    printf("fd %d:%s\n", 0, ttyname(0));
    printf("fd %d:%s\n", 1, ttyname(1));
    printf("fd %d:%s\n", 2, ttyname(2));
    return 0;
}
