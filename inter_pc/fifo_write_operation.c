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
    char buf[NUM] = "hello world";
    if ( (fd = open("./my_fifo", O_WRONLY)) < 0) {
        perror("open");
        exit(1);
    }
    printf("write data[%s] from FIFO\n", buf);
    write(fd, buf, sizeof(buf));

    return 0;
}
