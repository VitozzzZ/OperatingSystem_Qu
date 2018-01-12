#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
using namespace std;
struct student
{
   int number;
   char name[10];
   char sex[5];
   int age;
};
void Delete_stu(const char* filep, int n)  //从数组中删除第n个元素
{  
    char buf[5000];    
    int i = 0;   
    FILE* fp1 = fopen(filep, "r");        
    FILE* fp2 = fopen("temp.txt", "w");        
    while(fgets(buf,sizeof(buf), fp1))     //从filep中按行读取，并将值存到buff数组中 
    {   i++;
        if (i != n)              
        fprintf(fp2, "%s", buf);           //将filep文件第n行内容删除后存入temp.txt中   
    }   
    fclose(fp1);  
    fclose(fp2);  
    remove("stu.txt");                    //删除原文件
    rename("temp.txt","stu.txt");         //将临时文件名改为原文件名,
}  

void Add_stu(const char* filep, int n,student newvalue)  //在第n个元素前添加一个元素
{  
    char buf[5000];     
    FILE* fp1 = fopen(filep, "r");      
    FILE* fp2 = fopen("temp.txt", "w");      
    int i = 0;  
    while(fgets(buf,sizeof(buf), fp1))   //将filep文件第n行后加入一行数据后存入temp.txt中 
    {       
        i++;
        if (i == n)   
        {            
            fprintf(fp2,"%d\t%s\t%s\t%d\n",newvalue.number,newvalue.name,newvalue.sex,newvalue.age);
        }  
        else   
        {   
            fprintf(fp2, "%s", buf);   
        }  
    }   
    fclose(fp1);  
    fclose(fp2);  
    remove("stu.txt");
    rename("temp.txt","stu.txt");
}  

void readfile(const char* f)  //读取文件，验证函数
{
    FILE * fr=fopen(f,"r");
    assert(fr!=NULL);
    char buf[4096]; 
    while(fgets(buf, sizeof(buf), fr))  
     {  
        printf("%s", buf); 
     }
    fclose(fr);
}   


int main()
{
student s[10];
student newvalue={38,"lq","f",18};
FILE * stu=fopen("stu.txt","w+");
assert(stu!=NULL);
cout<<"Please input the value of ten students:"<<endl;
for(int i=0;i<10;i++)
{
   cin>>s[i].number>>s[i].name>>s[i].sex>>s[i].age;
   fprintf(stu,"%d\t%s\t%s\t%d\n",s[i].number,s[i].name,s[i].sex,s[i].age);
}
fclose(stu);
readfile("stu.txt");
cout<<endl;
assert(stu!=NULL);
cout<<"删除第三个学生信息之后的结果为"<<endl;
Delete_stu("stu.txt",3);
readfile("stu.txt");
cout<<endl;
assert(stu!=NULL);
cout<<"在第六个学生信息之后添加一个学生元素的结果为"<<endl;
Add_stu("stu.txt",6,newvalue);
readfile("stu.txt");
return 0;
}
