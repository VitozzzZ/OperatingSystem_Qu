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
   
