# 信号基础知识

## 信号产生

信号是软件中断，在头文件<signal.h>中，信号名都被定义为正整数常量

产生信号的方式：

1. 按键中断（硬中断）： ctrl + c等
2. 硬件异常产生信号：除数0，无效的内存引用，SIGSEGV信号
3. 进程使用kill(2)发送信号
4. 用户使用kill(1)将信号发送给进程

被core类型的信号中断后，会在本地生成core文件

core表示在当前工作目录的core文件中复制了该进程的内存映像，

```shell
kill -L # 显示所有的信号列表
zhuzs@gpu6964:~$ kill -L
 1) SIGHUP       2) SIGINT       3) SIGQUIT      4) SIGILL       5) SIGTRAP
 6) SIGABRT      7) SIGBUS       8) SIGFPE       9) SIGKILL     10) SIGUSR1
11) SIGSEGV     12) SIGUSR2     13) SIGPIPE     14) SIGALRM     15) SIGTERM
16) SIGSTKFLT   17) SIGCHLD     18) SIGCONT     19) SIGSTOP     20) SIGTSTP
......#还有其他的
ulimit -c # 设置core文件大小，如果不设置不保存core类型的中断内存映像
```

## 常用函数

### alarm

系统调用alarm的功能是设置一个定时器，当定时器计时到达时，将发出一个信号给进程。该调用的声明格式如下：

```c
#include <unistd.h>
unsigned int alarm(unsigned int seconds);
```

系统调用alarm安排内核为调用进程在指定的seconds秒后发出一个SIGALRM的信号。如果指定的参数seconds为0，则不再发送 SIGALRM信号。后一次设定将取消前一次的设定。该调用返回值为上次定时调用到发送之间剩余的时间，或者因为没有前一次定时调用而返回0。

```c
alarm(10) // 10秒后发送SIGALRM信号
unsigned int left_time = alar(0) //alarm剩余的时间，并且关闭发送SIGALRM信号
unsigned int left_time = alar(2) // 取消上一次设定，并返回上次剩余时间，重新定义alarm 2s
```

注意，在使用时，alarm只设定为发送一次信号，如果要多次发送，就要多次使用alarm调用，[代码测试](https://github.com/Jasonzhu0314/linux-system-programming/tree/main/signal_operation/alarmOpt.c)

### pause

用该函数可以造成进程主动挂起，等待信号唤醒。调用该系统调用的进程将处于阻塞状态(主动放弃cpu) 直到有信号递达将其唤醒。

```c
int pause(void);   
```

返回值：-1 ，并设置errno为EINTR

处理情况：

1. 如果信号的默认处理动作是终止进程，则进程终止，pause函数么有机会返回。
2. 如果信号的默认处理动作是忽略，进程继续处于挂起状态，pause函数不返回。
3. 如果信号的处理动作是捕捉，则【调用完信号处理函数之后，pause返回-1】errno设置为EINTR，表示“被信号中断”。想想我们还有哪个函数只有出错返回值。
4. pause收到的信号不能被屏蔽，如果被屏蔽，那么pause就不能被唤醒。

测试pause的阻塞和运行返回结果，[代码测试](https://github.com/Jasonzhu0314/linux-system-programming/tree/main/signal_operation/mySleep.c)

## 发送信号

1. shell命令

   kill - 发送信号

   ctrl + c  键盘命令，实际使用的是kill -SIGINT（多进程的情况下，会给父子进程都发送指令）

   ctrl + c 向程序发送键盘硬中断，程序从用户态跳转到内核态执行中断，PCB中更新信号，添加singint中断信号，然后切换到用户态，用户态查看PCB，有信号关闭程序，程序被关闭

2. 系统函数

   kill，可以给一个指定的进程发送指定的信号。

   raise， 发送信号，只能给自己发送信号。

   abort， 函数使当前进程接收到SIGABRT信号而异常终止。

   alarm等。。。还有很多



## 不可重入函数

经常会出现多个任务调用同一个函数的情况，如果有一个函数不幸被设计成为这样：那么不同任务调用这个函数时可能修改其他任务调用这个函数的数据，从而导致不可预料的后果。这样的函数是不安全的函数，也叫**不可重入函数**

> 当捕捉到信号时，不论进程的主控制流程当前执行到哪儿，都会先跳到信号处理函数中执行，从信号处理函数返回后再继续执行主控制流程。信号处理函数是个单独的控制流程，因为它和主控制流程是异步的，二者不存在调用和被调用的关系，并且使用不同的堆栈空间。引入了信号处理函数使得一个进程具有多个控制流程，如果这些控制流程访问相同的全局资源（全局变量、硬件资源等），就有可能出现冲突

常见的不可重入的函数

1. 使用静态数据结构，全局变量，如errno的值
2. 调用malloc或free
3. 使用标准I/O函数，标准I/O库的很多实现都以不可重入的方式使用数据结构
4. 有时候printf也不一定产生预期的结果


# 信号屏蔽

## 简介

> Unix环境高级编程10.8节内容
>
> 当一个信号产生时，内核通常在进程表中以某种形式设置一个标志。
>
> 当对信号采取了这种动作时，我们说向进程递送了一个信号。在信号产生（ generation）和递送（ delivery）之间的时间间隔内，称信号是未决的（ pending）。
>
> 进程可以选用“阻塞信号递送”。如果为进程产生了一个阻塞的信号，而且对该信号的动作是系统默认动作或捕捉该信号，则为该进程将此信号保持为未决状态，直到该进程对此信号解除了阻塞，或者将对此信号的动作更改为忽略。内核在递送一个原来被阻塞的信号给进程时（而不是在产生该信号时），才决定对它的处理方式。于是进程在信号递送给它之前仍可改变对该信号5的动作。进程调用 sigpending函数（见10.13节）来判定哪些信号是设置为阻塞并处于未决状态的。
>
> 如果在进程解除对某个信号的阻塞之前，这种信号发生了多次，那么将如何呢？POSX1允许系统递送该信号一次或多次。如果递送该信号多次，则称这些信号进行了排队。但是除非支持POSX1实时扩展，否则大多数UNix并不对信号排队，而是只递送这种信号一次。

## 信号屏蔽集

用于屏蔽阻塞信号的集合，当有信号传送过来时，会阻塞信号集

一个能表示多个信号——信号集（ signal set）的数据类型，信号种类数目可能超过一个整型量所包含的位数，所以一般而言，不能用整型量中的一位代表一种信号，也就是不能用一个整型量表示信号集。

```c
#include signal.h>
int sigemptyset(sigset t *set); // 清空
int sigfillset(signet t *set);
int sigaddset(Bigset_t *set, int signo); // 添加
int sigdelset(aigset_t *set, int signo); //删除
//四个函数的返回值：若成功则返回0，若出错则返回-1
int sigismember(const sigset_t *set, int signo); 
//返回值：若真则返回1，若假则返回0，若出错则返
```


## sigprocmask函数

一个进程的信号屏蔽字规定了当前阻塞而不能递送给该进程的信号集。调用函数 sigprocmask可以检测或更改其信号屏蔽字，或者在一个步骤中同时执行这两个操作。

```c
#include <signal.h>
int sigprocmask(int how, const sigset t *restrict set, sigset t *restrict oset );
//返回值：若成功则返回0，若出错则返回-1
```

1. 若oet是非空指针，那么进程的当前信号屏蔽字通过ose返回
2. 若set是一个非空指针，则参数hw指示如何修改当前信号屏蔽字。 SIG BLOCK是“或”操作，而 SIG SETMASK则是赋值操作。注意，不能阻塞SIGKILI和 SIGSTOP信号。


# 捕捉信号

不能被捕捉的信号：SIGKILL和SIGSTOP信号

## 标准库函数

**singal**函数，设置某一信号的对应动作

```c
#include <signal.h>
typedef void (*sighandler_t)(int); // 函数指针
sighandler_t signal(int signum, sighandler_t handler);
```

**参数**

第一个参数signum：指明了所要处理的信号类型，它可以取除了SIGKILL和SIGSTOP外的任何一种信号。 

第二个参数handler：描述了与信号关联的动作，它可以取以下三种值：（1）SIG_IGN(忽略)，（2）SIG_DFL (系统默认函数)（3）sighandler_t类型的函数指针 

>signal()会依参数signum 指定的信号编号来设置该信号的处理函数。当指定的信号到达时就会跳转到参数handler指定的函数执行。
>
>当一个信号的信号处理函数执行时，如果进程又接收到了该信号，该信号会自动被储存而不会中断信号处理函数的执行，直到信号处理函数执行完毕再重新调用相应的处理函数。但是如果在信号处理函数执行时进程收到了其它类型的信号，该函数的执行就会被中断。

**返回**

先前的信号处理函数指针，如果有错误则返回SIG_ERR(-1)。 

**核心代码**：自定义handler

```c
void my_handler() {
    return ;
}
signal(SIGALRM, my_handler); //将SIGALRM信号绑定为自定义的my_handler函数
```

## 系统调用函数
**sa_sigaction**

sigaction函数的功能是检查或修改与指定信号相关联的处理动作（或同时执行这两种操作）。此函数取代了UNX早期版本使用的 signal函数。

参数：

```shell
sa_handler此参数和signal()的参数handler相同，代表新的信号处理函数
sa_mask 用来设置在处理该信号时暂时将sa_mask 指定的信号集搁置
sa_flags 用来设置信号处理的其他相关操作。
SA_RESETHAND：当调用信号处理函数时，将信号的处理函数重置为缺省值SIG_DFL
SA_RESTART：如果信号中断了进程的某个系统调用，则系统自动启动该系统调用
SA_NODEFER ：一般情况下， 当信号处理函数运行时，内核将阻塞该给定信号。但是如果设置了 SA_NODEFER标记， 那么在该信号处理函数运行时，内核将不会阻塞该信号
```

使用前面介绍的alarm和pause函数，设计自己的mysleep函数，[代码](https://github.com/Jasonzhu0314/linux-system-programming/tree/main/signal_operation/mySleep.c)

## 常用特殊信号

### SIGCHLD
SIGCHLD，在一个进程终止或者停止时，将SIGCHLD信号发送给其父进程，按系统默认将忽略此信号，如果父进程希望被告知其子系统的这种状态，则应捕捉此信号，然后替子进程收尸，防止僵尸进程。

使用fork测试SIGCHLD信号，父进程非阻塞给子进程收尸，[代码](https://github.com/Jasonzhu0314/linux-system-programming/tree/main/signal_operation/childSignal.c)

## 实战

编写一个程序，实现：父进程创建两个子进程，然后父进程通过 signal来捕捉键盘发出的CTRL+C键鍵，当按下CTRL+C键后，父进程分别向两个子进程发出SIGUSR1和SIGUSR2，两个子进程收到信号后分别打印"child1 is killed by parent "，"child2 is killed by parent"。

**关键点：**

当终端发送CTRL+C时，父进程和子进程都会介绍到SIGINT信号
