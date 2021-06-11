# 线程

线程创建时并**不能保证**哪个线程会先运行：是新创建的线程，还是调用线程。新创建的线程可以访问进程的地址空间，并且继承调用线程的**浮点环境**和**信号屏蔽字**，但是该线程的挂起**信号集会被清除**。

## 创建线程

主线程终止，子线程也会被终止

```c
#include <pthread.h>
int pthread_create(pthread_t *restrict tidp,const pthread_attr_t *restrict_attr,void*（*start_rtn)(void*),void *restrict arg); 	// 创建线程
pthread_exit()		// 退出线程
pthread_join()     // 等待线程
pthread_cancel()	// 取消线程
pthread_t pthread_self() // 获取当前线程id， lu类型
```

## 退出线程

pthread_exit或者return返回的指针必须必须是全局的或者是malloc分配的，不能在线程函数的栈上分配，因为当其他线程得到这个返回值是，线程函数已经推出了。

**注意：**pthread_exit函数时，线程不会释放自己的资源，`man 2  pthread_exit`中写道` When a thread terminates, process-shared resources (e.g., mutexes, condition variables, semaphores, and file descriptors) are not released`

线程间通信的方式

1. 互斥量
2. 条件变量
3. 信号量
4. 读写锁

## 互斥量

当一个线程在修改变量的值时，其他线程在读取这个变量时可能会得到一个不一致的值。

一个典型的例子是，在一个多线程程序中，两个及以上个线程对同一个变量i执行i++操作，结果得到的值并不如顺序执行所预期的那样。

```c
#include <pthread.h>
pthread_mutex_t fastmutex = PTHREAD_MUTEX_INITIALIZER;  // 宏定义初始化
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);
int pthread_mutex_lock(pthread_mutex_t *mutex);  		// 上锁
int pthread_mutex_trylock(pthread_mutex_t *mutex); 		// 非阻塞上锁
int pthread_mutex_unlock(pthread_mutex_t *mutex);       // 放锁
int pthread_mutex_destroy(pthread_mutex_t *mutex);      // 删除
```

用程序修改变量值时所经历的三个步骤解释这个现象：

- 从内存单元读入寄存器
- 在寄存器中对变量操作（加/减1）
- 把新值写回到内存单元

注：同一个线程两次调用lock，会阻塞，死锁状态



## 条件变量

 在服务器编程中常用的线程池，多个线程会操作同一个任务队列，一旦发现任务队列中有新的任务，子线程将取出任务；Linux多线程之互斥锁有一个问题，没有获得锁的函数会一直尝试获取锁，这样很浪费CPU的时间，因此，需要有另外一种类型的同步，它允许一个线程（或进程）睡眠到发生某个事件为止。

而条件变量通过允许线程阻塞和等待另一个线程发送信号的方法弥补了互斥锁的不足，它常和互斥锁一起配合使用。使用时，条件变量被用来阻塞一个线程，当条件不满足时，**线程往往解开相应的互斥锁并等待条件发生变化**。一旦其他的某个线程改变了条件变量，他将通知相应的条件变量唤醒一个或多个正被此条件变量阻塞的线程。这些线程将重新锁定互斥锁并重新测试条件是否满足。一般说来，条件变量被用来进行线程间的同步。

条件变量类型`pthread_cond_t`

```c
#include <pthread.h>
int pthread_cond_init(pthread_cond_t *cv,const pthread_condattr_t *cattr); // 初始化
int pthread_cond_wait( pthread_cond_t *cptr, pthread_mutex_t *mptr);  // 等待条件变量
int pthread_cond_timedwait(pthread_cond_t *cv,pthread_mutex_t *mp, const structtimespec * abstime); // 等待条件变量到指定时间
int pthread_cond_signal( pthread_cond_t *cptr);	  // 通知单个
int pthread_cond_broadcast(pthread_cond_t *cv);   // 通知所有
int pthread_cond_destroy(pthread_cond_t *cv);    // 删除
```



## 信号量

semaphore

生产者线程用于往链表里添加节点，数个工作线程从链表取出节点并处理，对于一般的mutex_lock, mutex_unlock方式，如果这一段时间没有工作，那么工作线程将会不停的调用lock,unlock操作。而这样的操作毫无疑义。

信号灯其实就是一个计数器，也是一个整数。每一次调用wait操作将会使semaphore值减一，而如果semaphore值已经为0，则wait操作将会阻塞。每一次调用post操作将会使semaphore值加一。将这些操作用到上面的问题中。工作线程每一次调用wait操作，如果此时链表中没有节点，则工作线程将会阻塞，直到链表中有节点。生产者线程在每次往链表中添加节点后调用post操作，信号灯值会加一。这样阻塞的工作线程就会停止阻塞，继续往下执行。

信号灯的类型为`sem_t`。在声明后必须调用sem_init()。需要传递两个参数，第一个参数就是你之前声明的sem_t变量，第二个必须为0。当你不再需要信号灯时，你必须调用sem_destroy()来释放资源。

```c
#include <semaphore.h>
int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);   // 非阻塞的等
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);		// 定义时间等
int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_post(sem_t *sem);
```



## 读写锁

**读写锁机制：**

​	写者：写者使用写锁，如果当前没有读者，也没有其他写者，写者立即获得写锁；否则写者将等待，直到没有读者和写者。
​    读者：读者使用读锁，如果当前没有写者，读者立即获得读锁；否则读者等待，直到没有写者。

**读写锁特性：**

​	同一时刻只有一个线程可以获得写锁，同一时刻可以有多个线程获得读锁。读写锁出于写锁状态时，所有试图对读写锁加锁的线程，不管是读者试图加读锁，还是写者试图加写锁，都会被阻塞。读写锁处于读锁状态时，有写者试图加写锁时，之后的其他线程的读锁请求会被阻塞，以避免写者长时间的不写锁。

使用方法和互斥量类似，不同地方在于读锁可以多次上锁

