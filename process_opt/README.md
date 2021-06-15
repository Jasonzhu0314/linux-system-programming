# 进程控制

> 由于在fork之后经常跟随着exec，所以现在的很多实现并不执行一个父进程数据段、栈和堆的完全副本。作为替代，使用了写时复制（Copy-On-Writ,coW）技术。这些区域由父进程和子进程共享，而且内核将它们的访问权限改变为只读。如果父进程和子进程中的任一个试图修改这些区域，则内核只为修改区域的那块内存制作一个副本，通常是虚拟存储系统中的一“页”。

## 程序的内部空间

**内部分区**

使用`nm`命令查看可执行文件的文件区域如:`nm ./a.out`

1. 栈区，函数调用，局部变量

2. 堆区，程序员申请

3. 全局区：（1）初始化的全局变量和静态变量区**(.data)**，分为只读RO和可读写区RW（2）未初始化的全局变量和静态变量**(.bss)**，ZI（zero initialize)这个段在程序执行之前由内核初始化为0，所以不在可执行文件中占用位置，可减少可执行文件体积，（手动初始化为0的也在这个区域， 如：int a = 0）。

4. 文字常量区（静态存储区）.rodata区

   字符串会被编译器自动放在rodata中，加 **const** 关键字的常量数据会被放在 rodata 中

   只读，对于字符串常量，编译器会去掉重复的常量，让程序的每个字符串常量只有一份。

   在有的嵌入式系统中， rodata放在 ROM（或 NOR Flash）里，运行时直接读取，不须加载到RAM内存中。所以，在嵌入式开发中，常将已知的常量系数，表格数据等造表加以 const 关键字。存在ROM中，避免占用RAM空间

   ```c
   char *p = "abcde" // "abcde" 文字常量区, p在栈区
   *(p + 1)  = "x" // 执行错误，文字常量区只读
   char *p1 = "abcde" // "abcde" 文字常量区, p1在栈区， p和p1的地址一样
   
   char *p2[] = "abcde" // "abcde" 文字常量区, p2在栈区
   *(p2 + 1)  = "x" // 执行正确，将文字常量区的内容，复制到栈中
   ```

5. 程序代码区

   代码段（code segment/text segment）通常是指用来存放程序执行代码的一块内存区域。这部分区域的大小在程序运行前就已经确定，并且内存区域通常属于只读，某些架构也允许代码段为可写，即允许修改程序。在代码段中，也有可能包含一些只读的常数变量，例如字符串常量等。

**参考资料**

   [字符串变量初始化分析](https://blog.csdn.net/ywcpig/article/details/52303745)

   [程序内存管理](https://www.cnblogs.com/hythink/p/5422602.html)

   

## fork

**函数原型**

```c
#include <sys/types.h>
#include <unistd.h>
pid_t fork(void);
```

由fork创建的新进程被称为子进程（ child process）。fork函数被调用一次，但返回两次。两次返回的区别是子进程的返回值是0，而父进程的返回值则是新建子进程的进程ID。

子进程和父进程继续执行fork调用之后的指令。子进程是父进程的副本。例如，子进程获得父进程数据空间、堆和栈的副本。注意，这是子进程所拥有的副本。父进程和子进程并不共享这些存储空间部分。父进程和子进程共享正文段（程序只读部分）。

**fork失败**

1. 系统中已经有了太多的进程（通常意味着某个方面出了问题）
2. 该实际用户D的进程总数超过了系统限制，ulimit限制。

**两种用法：**

1. 一个父进程希望复制自己，使父进程和子进程同时执行不同的代码段。这在网络服务进程中是常见的—一父进程等待客户端的服务请求。当这种请求到达时，父进程调用fork，使子进程处理此请求。父进程则继续等待下一个服务请求。
2. 一个进程要执行一个不同的程序。这对shell是常见的情况。在这种情况下，子进程从fork返回后立即调用exec



## 僵尸进程

**父子进程的两种状态**

1. 父进程死了，子进程没死

   子进程由init进程收养，其操作过程大致是：在一个进程终止时，内核逐个检查所有活动进程，以判断它是否是正要终止进程的子进程，如果是，则该进程的父进程ID就更改为1（init进程的ID），这种处理方法保证了每个进程有一个父进程。

2. 父进程没死，子进程死了

   在UNX术语中，一个已经终止、但是其父进程尚未对其进行善后处理（获取终止子进程的有关信息、释放它仍占用的资源）的进程被称为**僵死进程（zombie）**ps（1）命令将僵死进程的状态打印为Z。

   内核为每个终止子进程保存了一定量的信息，所以当终止进程的父进程调用wait或 waitpid时，可以得到这些信息。这些信息至少包括进程ID、该进程的终止状态以及该进程使用的CPU时间总量。内核可以释放终止进程所使用的所有存储区，关闭其所有打开文件。

**危害**

如果编写一个长期运行的程序，它fork了很多子进程，那么除非父进程等待取得子进程的终止状态，不然这些子进程终止后就会变成僵死进程，信号部分会讲到如果非阻塞给子进程收尸。



## wait

**函数原型**

```c
#include <sys/types.h>
#include <sys/wait.h>
pid_t wait(int *wstatus);
pid_t waitpid(pid_t pid, int *wstatus, int options);
```

当一个进程正常或异常终止时，内核就向其父进程发送 **SIGCHLD信号**。因为子进程终止是个异步事件（这可以在父进程运行的任何时候发生），所以这种信号也是内核向父进程发的异步通知。父进程可以选择忽略该信号，或者提供一个该信号发生时即被调用执行的函数（信号处理程序）。对于这种信号的系统默认动作是忽略它。

1. 如果其所有子进程都还在运行，则**阻塞**。
2. 如果一个子进程已终止，正等待父进程获取其终止状态，则取得该子进程的终止状态立即返回。
3. 如果它没有任何子进程，则立即出错返回。



## exec

**函数原型**

```c
#include <unistd.h>
int execl(const char *path, const char *arg, ...);
int execlp(const char *file, const char *arg, ...);
int execle(const char *path, const char *arg, ..., char *const envp[]);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execve(const char *path, char *const argv[], char *const envp[]);
```

e：使用环境变量参数，envp

v: argv是一个数组，不使用的话，就是使用**函数变参**，数组的最后一个是**NULL**

p: （1）带p，当path中有/时，代表的是可执行文件，如果没有的话，就在系统环境的Path中寻找，可直接使用`ls`，可以修改envp，指向可行文件的目录（2）不带p，可使用相对路径，也可使用绝对路劲，比如说使用`ls`就不能用，要用`/bin/ls`。

> 对打开文件的处理与每个描述符的执行时关闭（ close-on-exec）标志值有关。对 FD CLOEXEC标志的说明，进程中每个打开描述符都有一个执行时关闭标志。若设置了此标志，则在执行exec时关闭该描述符；否则该描述符仍打开。除非特地用fcnt1设置了该执行时关闭标志，否则系统的默认操作是在exec后仍保持这种描述符打开。

当进程调用一种exec函数时，该进程执行的程序完全替换为新程序，而新程序则从其main函数开始执行。因为调用exec并不创建新进程，所以前后的进程ID并未改变。exec只是用磁盘上的一个新程序替换了当前进程的正文段、数据段、堆段和栈段

**返回**

exec函数族的函数执行成功后不会返回，只有调用失败了，它们才会返回一个-1，从原程序的调用点接着往下执行