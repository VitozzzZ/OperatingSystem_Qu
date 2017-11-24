#include <stdio.h>
#include "readbmp.hpp"
#include "stdlib.h"
#include "math.h"
#include <iostream>


#define LENGTH_NAME_BMP 30//bmp图片文件名的最大长度

using namespace std;

//变量定义

BITMAPINFOHEADER strInfo;

//显示位图文件头信息


void showBmpInforHead(tagBITMAPINFOHEADER pBmpInforHead){
    
    cout<<"位图宽:"<<pBmpInforHead.biWidth<<endl;
    cout<<"位图高:"<<pBmpInforHead.biHeight<<endl;
    cout<<"biBitCount采用颜色位数:"<<pBmpInforHead.biBitCount<<endl;
}


int main(){
    char strFile[LENGTH_NAME_BMP];//bmp文件名
    
    cout<<"请输入所要读取的文件名:"<<endl;
    cin>>strFile;
    FILE *fpi;
    fpi=fopen(strFile,"rb");
    if(fpi != NULL){
        //先读取文件类型
        WORD bfType;
        fread(&bfType,1,sizeof(WORD),fpi);
        if(0x4d42!=bfType)
        {
            cout<<"the file is not a bmp file!"<<endl;
            return NULL;
        }
        fread(&strInfo,1,sizeof(tagBITMAPINFOHEADER),fpi);
        showBmpInforHead(strInfo);//显示文件信息头
        fclose(fpi);
    }
    else
    {
        cout<<"file open error!"<<endl;
        return NULL;
    }
}
