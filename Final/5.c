#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <pthread.h>
using namespace std;
//内存 使用率
void *getWin_MemUsage(void *arg)
{
	MEMORYSTATUS ms; 
	::GlobalMemoryStatus(&ms);//GlobalMemoryStatus 为win32 API获取系统内存信息 
	cout<<"内存利用率为"<<ms.dwMemoryLoad<<"%"<<endl; 
}
__int64 CompareFileTime(FILETIME time1, FILETIME time2)//64位操作系统使用__int64，FILETIME 为文件时间 
{
	__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime;
	__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime;

	return (b - a);

}
//CPU使用情况
void * getWin_CpuUsage(void *arg)
{
	HANDLE hEvent;
	BOOL res;
	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;

	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
	preidleTime = idleTime;//空闲时间 
	prekernelTime = kernelTime;//一个程序读取并分析一个文件，读取时会调用系统接口，系统会调用驱动来从物理设备上读取数据的时间 
	preuserTime = userTime;//自己进程上下文中执行代码占用的时间就是user time 
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL); // 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled

	while (true)//使用循环，不停输出CPU利用率和空闲率 
	{
		WaitForSingleObject(hEvent, 1000);//windows API,间隔1000毫秒 
		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
		__int64 idle = CompareFileTime(preidleTime, idleTime);
		__int64 kernel = CompareFileTime(prekernelTime, kernelTime);
		__int64 user = CompareFileTime(preuserTime, userTime);
		__int64 cpu = (kernel + user - idle) * 100 / (kernel + user);
		//占用率=（总的时间-空闲时间）/总的时间 
		__int64 cpuidle = (idle)* 100 / (kernel + user);
		cout << "CPU利用率:" << cpu << "%" << " CPU空闲率:" << cpuidle << "%" << endl;
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;
	}
}
int main( int argc, char **argv )
{
	pthread_t id[2]; 
	//初始化RamThread和 CPUThread
	int r1=pthread_create(&id[1],0,getWin_MemUsage,NULL);
	if(r1!=0)
	{
		cout<<"create pthread error！"<<r1<<endl; 
	}
   int r0=pthread_create(&id[0],0,getWin_CpuUsage,NULL); 
	//获取内存使用率
	if(r0!=0)
	{
		cout<<"create pthread error！"<<r0<<endl; 
	}
	//获取CPU使用率
	pthread_exit(NULL);
	pthread_exit(NULL);
        return 0;
}
