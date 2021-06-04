/*************************************************************************
	> File Name: test.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Mon 31 May 2021 09:37:17 PM CST
    > explain: alarm函数的测试
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

int main() {

    alarm(3);
    int n = 5;
    while(n--) {
        // 重新定义alarm信号,定义为2s后惊醒，之前的alarm取消，并返回结果
        printf("left:%d\n", alarm(2));
        sleep(1);
        printf("sleep 1\n");
    }
    //程序在这个地方1s后,会被惊醒退出程序
    while(1);

    return 0;
}
