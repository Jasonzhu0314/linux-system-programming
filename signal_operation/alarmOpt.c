/*************************************************************************
	> File Name: alarmOpt.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Tue 01 Jun 2021 08:20:17 AM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>


int main() {
    alarm(5);
    sleep(3);
    
    unsigned int lef = alarm(3);

    return 0;
}
