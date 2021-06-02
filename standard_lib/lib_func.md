# 标准库函数与系统调用

fopen会返回FILE* 结构体指针，

FILE*中包含 用户程序读取的是buffer的读取位置

1. 缓冲区
2. 读写位置
3. FD（文件描述符）



## 文件缓冲

分为三种：

1. 全缓冲
2. 行缓冲（出现换行，或者缓冲区满，标准输出）
3. 无缓冲（网络，标准错误输出）

```c++
#include <stdio.h>
int main() {
    fputc("A", STDIN);
    
    fputc("\n", STDIN); //只有调用这句话，才会在屏幕中打印字符"A"
    while (1) {
        ;
    }
    
    return 0;
}
```



## 文件描述符表

每个进程在Linux内中都有一个task_struct(进程控制块)来维护进程相关的信息，称为进程控制块PCB 中包含有files_struct(文件描述符表)，其中每个表象包含一个已经打开的文件的指针，用户程序不能直接访问内核中的文件描述符表，用户通过文件描述符表的索引，控制文件，这些索引就称为文件描述符，

程序开始启动时会**自动**打开三个文件，标准输入，标准输出， 标准错误输出

分别对应的文件描述符0，1， 2，这就是为什么socket的文件描述符返回结果是3



## open

open函数可以打开或创建一个文件

```c++
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);
// 返回值，成功返回新分配的文件描述符，出错返回-1并设置errno
```

flags:必选项， 一下三个常数中必须指定一个，且仅允许指定一个

```shell
O_WRONLY 只写
O_RDONLY 只读
O_RDWR 可读可写
```

可选项：

1. O_CREAT，若文件不存在，创建文件如果要使用的话，一定要设置mode，mode指的就是文件的操作权限，可以使用**0644**这种，也可以使用指定的宏定义相或的方式。
2. O_EXCL，确定创建文件，如果文件已经存在，程序报错。
3. O_TRUNC,  截断文件，也就是清空文件。



## close

```c++
#include <unistd.h>
int close(int fd);
// 返回值：成功返回0， 出错返回-1 并
```

当一个进程终止时，内核对该进程所有尚未关闭的文件描述符调用close关闭，所以即使用户程序不调用close，在终止时内核也会自动关闭它打开的所有文件

由open返回的文件描述符一定是该进程尚未使用的最小的文件描述符



## read

```c++
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);
```

使用C标准I/O库时的读写位置是用户空间I/O缓冲区

实际读到的字节数会小于请求读的字节数count，

1. 读常规文件时，在读到count个字节之前已到达文件末尾
2. 从终端设备读，通常以行位单位，读到换行符就返回了





## 高级控制

### glibc标准函数库

标准库的读写位置是缓冲区。

fseek

fopen

fputc

fgetc

fclose



### lseek

O_APPEND，追加的方式打开。内核的读写位置是文件区



exit和return的区别

exit 退出程序， `echo $?`查看程序退出的状态

return直结束当前函数







