/*************************************************************************
	> File Name: writeOpt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Thu 27 May 2021 04:02:28 PM CST
 ************************************************************************/

#include <unistd.h>

int main() {
    
    write(2, "hello", 5);
    while(1);


    return 0;
}

