//file copy program

#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 4096
#define OUTPUT_MODE 0700

int main(int argc,char**argv){
    int in_fd,out_fd,rd_count,wt_count;
    char buffer[BUF_SIZE];
    
    if(argc!=3) exit(1);
    //Open the input file and create the output file
    in_fd=open(argv[1],O_RDONLY);     //open the source file
    if(in_fd<0) exit(2);
    out_fd=creat(argv[2],OUTPUT_MODE);//create the destination file
    if(out_fd<0) exit(3);
    //Copy loop
    while(true){
        rd_count=read(in_fd,buffer,BUF_SIZE);
    
    if(rd_count<=0) break;
        wt_count=write(out_fd,buffer,rd_count);
        if(wt_count<=0) exit(4);
    }
    close(in_fd);
    close(out_fd);
    if(rd_count==0)
        exit(0);   //no error on last read
    else
        exit(5);   //error on last read
    
}

#include <stdio.h>
#include <stdlib.h>

int copyFile(char *fileRead, char *fileWrite);

int main(){
    char fileRead[100];  // 要复制的文件名
    char fileWrite[100];  // 复制后的文件名
   
    // 获取用户输入
    printf("要复制的文件：");
    scanf("%s", fileRead);
    printf("将文件复制到：");
    scanf("%s", fileWrite);

    // 进行复制操作
    if( copyFile(fileRead, fileWrite) ){
        printf("恭喜你，文件复制成功！\n");
    }else{
        printf("文件复制失败！\n");
    }

    return 0;
}

/**
* 文件复制函数
* @param    fileRead    要复制的文件
* @param    fileWrite   复制后文件的保存路径
* @return   int         1: 复制成功；2: 复制失败
**/
int copyFile(char *fileRead, char *fileWrite){
    FILE *fpRead;  // 指向要复制的文件
    FILE *fpWrite;  // 指向复制后的文件
    int bufferLen = 1024*4;  // 缓冲区长度
    char *buffer = (char*)malloc(bufferLen);  // 开辟缓存
    int readCount;  // 实际读取的字节数

    if( (fpRead=fopen(fileRead, "rb")) == NULL || (fpWrite=fopen(fileWrite, "wb")) == NULL ){
        printf("Cannot open file, press any key to exit!\n");
        getch();
        exit(1);
    }

    // 不断从fileRead读取内容，放在缓冲区，再将缓冲区的内容写入fileWrite
    while( (readCount=fread(buffer, 1, bufferLen, fpRead)) > 0 ){
        fwrite(buffer, readCount, 1, fpWrite);
    }

    free(buffer);
    fclose(fpRead);
    fclose(fpWrite);

    return 1;
}
