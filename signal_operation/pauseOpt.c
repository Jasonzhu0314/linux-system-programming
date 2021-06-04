/*************************************************************************
	> File Name: pauseTest.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Thu 03 Jun 2021 06:10:47 AM CST
    > explain: pause和signal函数的测试
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void show_handdler(int num) {
    printf("alarm\n");
    return ;
}

int main() {

    //signal(SIGALRM, show_handdler);
    alarm(2);
    //if (pause() < 0) { // 程序会阻塞在这里，等待信号
    //    perror("pause:");
    //    exit(1);
    //};
    struct sigaction new_act, old_act;
    new_act.sa_handler = show_handdler;  // 指向自定义函数
    //标准库函数可使用singal(SIGALRM, _handler);
    sigemptyset(&new_act.sa_mask);  // 初始化
    sigaddset(&new_act.sa_mask, SIGALRM);  //将SIGALRM信号添加到信号屏蔽字中阻塞，在函数调用时，信号被屏蔽
    new_act.sa_flags = 0;
    sigaction(SIGALRM, &new_act, &old_act);
    
    pause();
    while(1) {
        sleep(1);
        printf("sleep1:\n");
    };
    
    return 0;
}
