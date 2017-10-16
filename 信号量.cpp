#include <iostream>
#include <conio.h>         //getch()函数 
#include<semaphore.h>      //信号量所在 
#include "windows.h" 
using namespace std;
sem_t sem;                 //信号量定义为全局变量 
DWORD WINAPI funproc(LPVOID lpparentet);     //子线程执行函数声明 
int main()  
{  
	sem_init(&sem,0,0);    //信号量初始化，第一个参数为初始化对象，最后一个为初值 
	CreateThread(NULL,0,funproc,NULL,0,NULL);   //创建子线程，第三个参数为所调用函数 
    while(getch()!='e')
	{
		sem_post(&sem);     //V函数，信号量加一 
	} 
    return 0;  
}
DWORD WINAPI funproc(LPVOID lpparentet)     //子线程执行函数
{
	while(1)
	{
		sem_wait(&sem);            //P函数 
		cout<<"hello"<<endl;
	} 
