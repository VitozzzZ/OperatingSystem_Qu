//linux下创建一个子进程，父进程等待子进程输出100遍hello后输出over


#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<wait.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
int main(int argc , char **argv){
    pid_t fpid;
    fpid=fork();
    int status=-1;
    if(fpid<0){
        printf("error in fork");
    }
    else if(fpid==0){
        for(int i=0;i<100;i++){
            printf("hello\n");
        }
        exit(5);
    }
    else{
        wait(&status);
        printf("Over\n");
    }
}
/*
众所周知，wait()函数一般用在父进程中等待回收子进程的资源，而防止僵尸进程的产生。

(In UNIX System terminology, a process that has terminated, but whose parent has not yet waited for it, is called a zombie. )

 

下面我就带着问题，层层深入地来分析这个函数。

当一个进程正常或异常终止时，内核就向其父进程发送SIGCHLD信号。
因为子进程终止是个异步事件(这可以在父进程运行的任何是否发生)，所以这种信号也是内核向父进程发的异步通知。
父进程可以选择忽略该信号，或者提供一个信号处理程序。
对于这种信号的系统默认动作是忽略。

调用wait()或waitpid()的父进程会发生什么情况：
a. 如果其所有子进程都还在运行，则阻塞;
　　Q1:如果是一部分子进程终止，而另一部分还在运行，那么父进程还会阻塞吗？
　　　　不会，只要有一个进程终止，wait就会返回。也就是说只要wait接收到一个SIGCHLD信号,wait()就会返回。对于两个或多个子进程的情况,需要调用wait两次或多次
b. 如果一个子进程已经终止，正等待父进程获取其终止状态，则取得该子进程的终止状态立即返回;
c. 如果它没有任何子进程，则立即出错返回;

int wait(int* statloc);
int waitpid(pid_t pid, int* statloc, int options);

这两个函数的区别如下：
1. 在一个子进程终止前，wait使其调用者阻塞，而waitpid有一个选项，可使调用者不阻塞;
2. waitpid()并不等待在其调用之后的第一个终止的子进程，它有若干个选项，可以控制它所等待的进程;

 

如果一个子进程已经终止,并且是一个僵尸进程,则wait立即返回并取得该子进程的终止状态,否则wait使其调用者阻塞直到一个子进程终止。
如果调用者阻塞而且它有多个子进程,则在其一个子进程终止时,wait就立即返回。
因为wait的返回值是终止进程的进程ID,所以父进程总能知道哪一个子进程终止了。

参数statloc如果不是一个空指针,则终止进程的终止状态就存放在statloc所指向的单元。
参数statloc如果是一个空指针,则表示父进程不关心子进程的终止状态。

Q2:statloc中不同的值具体表示什么含义呢？
wait的输出参数statloc中，某些位表示退出状态(正常返回),其它位则指示信号编号(异常返回),有一位指示是否产生了一个core文件等等。

有四个互斥的宏可用来取得进程终止的原因。
1. WIFEXITED(status):若为正常终止子进程返回的状态,则为真。WEXITSTATUS(status)可取得子进程传送给exit、_exit或_Exit参数的低8位;
2. WIFSIGNALED(status):若为异常终止子进程返回的状态,则为真(接到一个不捕捉的信号,如SIGCHLD)。对于这种情况，可执行WTERMSIG(status),取得是子进程终止的信号的编号。
另外，有些实现(非Single UNIX Specification)定义宏WCOREDUMP(status),若已产生终止进程的core文件,则它返回真。
3. WIFSTOPPED(status):若为当前暂停的子进程的返回的状态,则为真。对于这种情况,可执行WSTOPSIG(status),取使子进程暂停的信号编号。
4. WIFCONTINUED(status):若在作业控制暂停后已经继续的子进程返回了状态,则为真.(POSIX.1的XSI扩展,仅用于waitpid)

 

Q3: wait函数和SIGCHLD信号的关系?两者之间的关系，需要分成三个问题

已知系统默认是忽略SIGCHLD信号，在一个进程终止或停止时，会将SIGCHLD信号发送给其父进程。
已知父进程若不调用wait()获取子进程的终止状态,那么子进程就会变成僵尸进程。

Q3.1:wait()是关于是否产生僵尸进程的问题。

Q3.2:SIGCHLD信号是关于自己本身的处理方式的选择问题。

当这两个问题(Q3.1&Q3.2)结合在一起应用时，就产生了另外一个问题，父进程是同步还是异步的问题(或者描述为阻塞还是非阻塞问题)
当SIGCHLD的处理方式是系统默认时，父进程调用了wait()以防止子进程变成僵尸进程，那么父进程必须等待子进程结束之后才能执行wait()之后的流程，即同步问题。
当SIGCHLD的处理方式是捕获时，在其信号处理程序中调用wait()函数，就能获取子进程的终止状态而不产生僵尸进程同时父进程并不会阻塞，做自己想做的事，即异步问题。

Q3.3:wait什么时候返回的问题，wait()的返回和SIGCHLD有什么关系？

根据wait()的描述：
All of these system calls are used to wait for state changes in a child of the calling process, and obtain information about the child whose
state has changed. A state change is considered to be: the child terminated; the child was stopped by a signal; or the child was resumed by a
signal. In the case of a terminated child, performing a wait allows the system to release the resources associated with the child; if a wait is
not performed, then terminated the child remains in a "zombie" state (see NOTES below).

If a child has already changed state, then these calls return immediately. Otherwise they block until either a child changes state or a signal
handler interrupts the call (assuming that system calls are not automatically restarted using the SA_RESTART flag of sigaction(2)). In the
remainder of this page, a child whose state has changed and which has not yet been waited upon by one of these system calls is termed waitable.
当子进程的状态发生改变时，wait()返回;
当调用wait()的进程接收到一个被设置为SA_INTERRUPT的信号时，wait()返回;
因为SIGCHLD信号的产生必然是伴随着子进程状态的改变，所以当有SIGCHLD信号发生时，wait会返回。*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

void print_exit(int status)
{
    if (WIFEXITED(status))
        printf("normal termination, exit status = %d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("abnormal termination, signal number = %d%s\n", WTERMSIG(status),
#ifdef WCOREDUMP
        WCOREDUMP(status) ? ("core file generated") : (""));
#else
    "");
#endif
    else if (WIFSTOPPED(status))
        printf("child stopped, signal number=%d\n", WSTOPSIG(status));
}

void sig_child(int signo)
{
    int status;
    int ret;
    ret = wait(&status);
    printf("pid:%d, res:%d, status=%d, %s\n", getpid(), ret, status, strerror(errno));
    print_exit(status);
}

void sig_usr(int signo)
{
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
    else if (signo == SIGUSR2)
        printf("received SIGUSR2\n");
    else
        printf("received signal %d\n", signo);
}


int main(int argc, char** argv)
{
    pid_t pid;
    int status;
    int ret;
    int remaintime=0;
    struct sigaction act, oact;
    sigset_t oldmask;

    //signal(SIGCHLD, sig_child);
    //signal(SIGUSR1, sig_usr);
    
    act.sa_handler = sig_usr;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0|SA_INTERRUPT;
    sigaction(SIGUSR1, &act, &oact);

    if ((pid=fork()) < 0)
    {
        printf("fork error\n");
        return -1;
    }
    else if (pid == 0)
    {

        printf("child:pid:%d\n", getpid());
        remaintime = sleep(200);
        printf("remiantime=%d\n", remaintime);
        //exit(0);    
        //return 0;
        //
        //sleep(30);//SIGQUIT
    }
    else
    {
        printf("father:pid:%d\n", getpid());
        //while(1)
        //{
        //    sleep(1);
        //    printf("1111\n");
        //}
        ret = wait(&status);
        printf("res:%d, status=%d, %s\n", ret, status, strerror(errno));
        print_exit(status);
    }

    return 0;
}
