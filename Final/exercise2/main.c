#include<stdio.h>
#include<curses.h>
#include<stdlib.h>
#include<math.h>
#include "FileCopy.h"
#include "FileInsert.h"
#include "FileDelete.h"

#define N 10



struct stu{
    int num;  //学号
    char name[10]; //姓名
    char sex[5];   //性别
    int age;  //年龄
    
}stu1[N], stu2[N], *p1, *p2;

int main(){
    //1 从键盘输入10个学生的信息
    FILE *fp,*fp2;
    int i;
    p1=stu1;
    p2=stu2;
    fp=fopen("/home/zsq/2.txt","wb+");
    for(i=0;i<N;i++,p1++){
        scanf("%d %s %s %d",&p1->num,p1->name,p1->sex,&p1->age);
    }
    fwrite(stu1, sizeof(struct stu), N, fp);
    fclose(fp);
    
    //2 删除第3个学生的信息
    fp=fopen("/home/zsq/2.txt", "wb+");
    fdelete(fp, 2*sizeof(struct stu), sizeof(struct stu));
    fclose(fp);
    
    //3 在第六个学生信息后插入一个新的学生信息
    fp=fopen("/home/zsq/2.txt", "wb+");
    fp2=fopen("/home/zsq/2.1.txt", "wb+");
    finsert(fp, 6*sizeof(struct stu), fp2, sizeof(struct stu));
    fclose(fp);
    fclose(fp2);
    
    //4 验证函数
    fp=fopen("/home/zsq/2.txt","wb+");
    fread(stu2, sizeof(struct stu), N+1, fp);
    for(i=0;i<N+1;i++,p2++){
        printf("%d %s %s %d",p2->num,p2->name,p2->sex,p2->age);
    }
    fclose(fp);
    
}
