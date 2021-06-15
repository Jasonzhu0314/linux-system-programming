/*************************************************************************
	> File Name: fifo_operation.c
	> Author:Jasonzhu 
	> Mail: jasonzhu_0314@163.com
	> Created Time: Sat 12 Jun 2021 07:58:03 AM CST
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM 20

int main() {
    int ret;
    // 创建命名管道
    //ret = mkfifo("/home/zhuzs/project/linux-system-programming/inter_pc/my_fifo", 0666);
    ret = mkfifo("./my_fifo", 0666);
    if (ret) {
        perror("mkfifo");
    }
    int n, fd;
    char buf[NUM];
    if ( (fd = open("./my_fifo", O_RDONLY)) < 0) {
        perror("open");
        exit(1);
    }
    if ( (n = read(fd, buf, sizeof(buf))) < 0) {
        perror("read");
        exit(2);
    }
    printf("get data[%s] from FIFO\n", buf);

    return 0;
}
