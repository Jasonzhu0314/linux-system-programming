/*************************************************************************
	> File Name: pauseOpt.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Wed 02 Jun 2021 08:10:25 AM CST
    > explain: 使用alarm和pause设计自己的sleep函数
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void _handler() {
    return ;
}

//使用alarm和pause设计自己的sleep函数
unsigned int mysleep(unsigned int sec) {
    
    sigset_t new_mask, old_mask, sus_mask;
    //将信号屏蔽字添加到旧的信号屏蔽中
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask); 

    //修改信号处理函数
    struct sigaction new_act, old_act;
    new_act.sa_handler = _handler;  // 指向自定义函数
    //标准库函数可使用singal(SIGALRM, _handler);
    sigemptyset(&new_act.sa_mask);  // 初始化
    sigaddset(&new_act.sa_mask, SIGALRM);  //将SIGALRM信号添加到信号屏蔽字中阻塞，在函数调用时，信号被屏蔽
    new_act.sa_flags = 0;
    sigaction(SIGALRM, &new_act, &old_act);
    
    alarm(sec);
    sus_mask = old_mask;

    sigdelset(&sus_mask, SIGALRM);
    //原子操作，防止信号在pause之前已经处理，执行中sigsuspend函数，接收alarm信号
    sigsuspend(&sus_mask);

    //返回剩余alarm的剩余时间
    unsigned int unsleep = alarm(0);

    //恢复初始现场
    sigaction(SIGALRM, &old_act, NULL);
    sigprocmask(SIG_SETMASK, &old_mask, NULL);

    return unsleep;
}

int main() {
    int n = 10;
    while(n) {
        mysleep(1);
        printf("my sleep 1 s: %d\n", n);
        n--;
    }

    return 0;
}
