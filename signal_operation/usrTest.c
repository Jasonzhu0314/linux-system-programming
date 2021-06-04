   
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>    

void Sigusr1Handler(int signo) {
    printf("shoudaoi\n");
    return ; 
}

void SigintHandler(int signo) {
    printf("SigintHandler\n");
    kill(getpid(), SIGUSR1);
    return ; 
}

int main() {       
    struct sigaction act, old_act;
    act.sa_flags = 0;
    act.sa_handler = Sigusr1Handler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGUSR1);

    sigaction(SIGUSR1, &act, &old_act);
    kill(getpid(), SIGUSR1);

    act.sa_flags = 0;
    act.sa_handler = SigintHandler;
    sigemptyset(&act.sa_mask);

    sigaction(SIGINT, &act, &old_act);
    kill(getpid(), SIGINT);


    while(1);
}