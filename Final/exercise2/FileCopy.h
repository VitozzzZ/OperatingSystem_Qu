/**
 * 文件复制函数
 * @param  fSource       要复制的原文件
 * @param  offsetSource  原文件的位置偏移（相对文件开头），也就是从哪里开始复制
 * @param  len           要复制的内容长度，小于0表示复制offsetSource后边的所有内容
 * @param  fTarget       目标文件，也就是将文件复制到哪里
 * @param  offsetTarget  目标文件的位置偏移，也就是复制到目标文件的什么位置
 * @return  成功复制的字节数
 **/
long fcopy(FILE *fSource, long offsetSource, long len, FILE *fTarget, long offsetTarget){
    int bufferLen = 1024*4;  // 缓冲区长度
    char *buffer = (char*)malloc(bufferLen);  // 开辟缓存
    int readCount;  // 每次调用fread()读取的字节数
    long nBytes = 0;  //总共复制了多少个字节
    int n = 0;  //需要调用多少次fread()函数
    int i;  //循环控制变量
    
    fseek(fSource, offsetSource, SEEK_SET);
    fseek(fTarget, offsetTarget, SEEK_SET);
    
    if(len<0){  //复制所有内容
        while( (readCount=fread(buffer, 1, bufferLen, fSource)) > 0 ){
            nBytes += readCount;
            fwrite(buffer, readCount, 1, fTarget);
        }
    }else{  //复制len个字节的内容
        n = (int)ceil((double)((double)len/bufferLen));
        for(i=1; i<=n; i++){
            if(len-nBytes < bufferLen){ bufferLen = len-nBytes; }
            readCount = fread(buffer, 1, bufferLen, fSource);
            fwrite(buffer, readCount, 1, fTarget);
            nBytes += readCount;
        }
    }
    fflush(fTarget);
    free(buffer);
    return nBytes;
}
