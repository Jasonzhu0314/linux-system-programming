/*************************************************************************
	> File Name: charOpt.c
	> Author:Jasonzhu 
	> Mail: 
	> Created Time: Sun 30 May 2021 10:26:32 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main() {

    int i = 0;
    int *arr[10] = {0};
    for(;i < 5; i++) {
        int* a = (int *) malloc(sizeof(int));
        *a = 10;
        *a = *a - i; 
        arr[i] = a;
        printf("%p\n", a);
    }
    for (i = 0;i < 5; i++) {
        printf("arr[%d]:%d\n", i, *arr[i]);
    }
    


    return 0;
}

