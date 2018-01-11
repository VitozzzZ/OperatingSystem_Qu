#include <io.h>
#include <stdio.h>
#include<iostream>
#include<cstring>
using namespace std;
int main()
{
	void ReadFileDir(string path);
    ReadFileDir("c:");  
    return 0;
}
void ReadFileDir(string path)   
{ 
struct _finddata_t FileInfo;
   string path_=path+"\\*"; 
	long result = _findfirst(path_.c_str(),&FileInfo);     
    if (result<0)  
    {
        cout<<"搜索失败，没有找到匹配文件项目！"<<endl; 
    }
    while ( _findnext(result, &FileInfo) == 0)
	{
        if (FileInfo.attrib & _A_SUBDIR)    
        {
            if( (strcmp(FileInfo.name,".") != 0 ) &&(strcmp(FileInfo.name,"..") != 0))  
            {
                string newPath=path+"\\"+FileInfo.name;
                ReadFileDir(newPath);
            }
        }
        else  
        {
            cout <<path<< "\\" << FileInfo.name  << " "<<endl;
        }
    } 
    _findclose(result);    
}
