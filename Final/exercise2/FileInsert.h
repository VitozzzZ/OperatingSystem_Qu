long fsize(FILE *fp);

long fsize(FILE *fp){
    long n;
    fpos_t fpos;  //当前位置
    fgetpos(fp, &fpos);  //获取当前位置
    fseek(fp, 0, SEEK_END);
    n = ftell(fp);
    fsetpos(fp,&fpos);  //恢复之前的位置
    return n;
}

/**
 * 向文件中插入内容
 * @param  fp      要插入内容的文件
 * @param  buffer  缓冲区，也就是要插入的内容
 * @param  offset  偏移量（相对文件开头），也就是从哪里开始插入
 * @param  len     要插入的内容长度
 * @return  成功插入的字节数
 **/
int finsert(FILE *fp, long offset, void *buffer, int len){
    long fileSize = fsize(fp);
    FILE *fpTemp;  //临时文件
    
    if(offset>fileSize || offset<0 || len<0){  //插入错误
        return -1;
    }
    
    if(offset == fileSize){  //在文件末尾插入
        fseek(fp, offset, SEEK_SET);
        
    
        if(!fwrite(buffer, len, 1, fp)){
            return -1;
        }
    }
    
    if(offset < fileSize){  //从开头或者中间位置插入
        fpTemp = tmpfile();
        fcopy(fp, 0, offset, fpTemp, 0);
        fwrite(buffer, len, 1, fpTemp);
        fcopy(fp, offset, -1, fpTemp, offset+len);
        freopen("/home/zsq/2.txt", "wb+", fp );
        fcopy(fpTemp, 0, -1, fp, 0);
        fclose(fpTemp);
    }
    
    return 0;
}
