/*
Windows上C++使用共享内存进行进程间通讯

共享内存 (也叫内存映射文件) 主要是通过映射机制实现的 , Windows 下进程的地址空间在逻辑上是相互隔离的 , 但在物理上却是重叠的 ; 
所谓的重叠是指同一块内存区域可能被多个进程同时使用 , 当调用 CreateFileMapping 创建命名的内存映射文件对象时 , Windows即在物
理内存申请一块指定大小的内存区域,返回文件映射对象的句柄 hMap;为了能够访问这块内存区域必须调用 MapViewOfFile 函数,促使
Windows 将此内存空间映射到进程的地址空间中 ; 当在其他进程访问这块内存区域时 , 则必须使用 OpenFileMapping 函数取得对象句柄 
hMap , 并调用 MapViewOfFile 函数得到此内存空间的一个映射,这样系统就把同一块内存区域映射到了不同进程的地址空间中,从而达到共享内存的目的
*/
//进程 A 将数据写入到共享内存 :
#include "stdafx.h"  //http://blog.csdn.net/lijun5635/article/details/13090341/
#include <windows.h>
using namespace std;

#define BUF_SIZE 4096

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[]){
    //定义共享数据
    char szBuffer[]="Hello Shared Memory";
    
    //创建共享文件句柄
    HANDLE hMapFile=createFileMapping(INVALID__HANDLE_VALUE,NULL,PAGE_READWRITE,0,BUF_SIZE,L"ShareMemory");
    //从左到右依次是物理文件句柄；默认安全级别；可读可写；高位文件大小；地位文件大小；共享内存名称
    
    //映射缓存区视图，得到指向共享内存的指针
    LPVOID lpBase = MapViewOfFile(hMapFile/*共享内存的句柄*/,FILE_MAP_ALL_ACCESS/*可读可写许可*/,0,0,BUF_SIZE);
    
    //将数据拷贝到共享内存
    strcpy((char*)lpBase,szBuffer);
    
    // 线程挂起等其他线程读取数据
    Sleep(20000);
    
    // 解除文件映射
    UnmapViewOfFile(lpBase);
    // 关闭内存映射文件对象句柄
    CloseHandle(hMapFile);
    return 0;
    
}
//进程 B 获取共享内存中的数据 :
#include "stdafx.h"
#include <iostream>  
#include <windows.h>  
using namespace std;  

#define BUF_SIZE 4096

int main()  
{  
    // 打开共享的文件对象
    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,NULL,L"ShareMemory");
    if (hMapFile)
    {
        LPVOID lpBase = MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,0);
        // 将共享内存数据拷贝出来
        char szBuffer[BUF_SIZE] = {0};
        strcpy(szBuffer,(char*)lpBase);
        printf("%s",szBuffer);

        // 解除文件映射
        UnmapViewOfFile(lpBase);
        // 关闭内存映射文件对象句柄
        CloseHandle(hMapFile);
    }
    else
    {
        // 打开共享内存句柄失败
        printf("OpenMapping Error");
    }
    return 0;
}
