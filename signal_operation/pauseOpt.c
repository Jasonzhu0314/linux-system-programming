/*************************************************************************
	> File Name: pauseOpt.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Wed 02 Jun 2021 08:10:25 AM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void _handler() {
    return ;
}

unsigned int mysleep(unsigned int sec) {
    sigset_t new_mask, old_mask, sus_mask;
    struct sigaction new_act, old_act;

    new_act.sa_handler = _handler;
    new_act.sa_flags = 0;
    
    sigaction(SIGALRM, &new_act, &old_act);
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGALRM);

    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);
    
    alarm(sec);
    sus_mask = old_mask;
    sigdelset(&sus_mask, SIGALRM);
    sigsuspend(&sus_mask);

    unsigned int unsleep = alarm(0);

    sigaction(SIGALRM, &old_act, NULL);
    sigprocmask(SIG_SETMASK, &old_mask, NULL);

    return unsleep;
}

int main() {
    int n = 10;
    while(n) {
        mysleep(1);
        printf("my sleep 1 s");
        n--;
    }

    return 0;
}
