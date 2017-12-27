//popen() 函数用于创建一个管道：其内部实现为调用 fork 产生一个子进程，执行一个 shell 以运行命令来开启一个进程这个进程必须由 pclose() 函数关闭。
#include “stdio.h”
#include “stdlib.h”
int main(){
    FILE *fp;
    char buf[200] = {0};
    if((fp = popen(“cat > test″, “w”)) == NULL) {
        perror(“Fail to popen\n”);
        exit(1);
    }
    fwrite(“Read pipe successfully !”, 1, sizeof(“Read pipe successfully!”), fp);
    pclose(fp);
    return 0;
}
//执行完毕后，当前目录下多了一个test文件，打开，里面内容为Read pipe successfully !

#include “stdio.h”
#include “stdlib.h”
int main(){
    FILE *fp;
    char buf[200] = {0};
    if((fp = popen(“cat test”, “r”)) == NULL) {
        perror(“Fail to popen\n”);
        exit(1);
        
    }
    while(fgets(buf, 200, fp) != NULL) {
        printf(“%s”, buf);
    }
    pclose(fp);
    return 0;
}
//打印输出： Read pipe successfully !
