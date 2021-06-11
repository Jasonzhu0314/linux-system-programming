/*************************************************************************
	> File Name: messageQueue.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Mon 31 May 2021 09:04:18 AM CST
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int main() {

    key_t  key = ftok("./queue", 9 );
    printf("key=%#x\n", key);
    int mqid = msgget(key, IPC_CREAT | 0666);
    printf("mqid=%d\n", mqid);

    return 0;
}


