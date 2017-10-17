//用信号量解决
#define N 100            //缓冲区槽的数目
typedef int semaphore;   //信号量是一种特殊的整形数据
semaphore mutex=1;
semaphore empty=N;
semaphore full=0;

void producer(void){
    int item;
    while(true){
        item=produce_item;   //产生数据
        down(&empty);
        down(&mutex);        //进入临界区
        insert_item(item);   //将新数据项放入缓冲区
        up(&mutex);          //离开临界区
        up(&full);
    }
}

void consumer(void){
    int item;
    while(true){
        
        down(&full);
        down(&mutex);        //进入临界区
        remove_item(item);   //从缓冲区取出数据项
        up(&mutex);          //离开临界区
        up(&empty);
        consume_item(item);
    }
}

//pthread中的互斥量
#include<stdio.h>
#include<pthread.h>
#define MAX 1000000000  //需要生产的数量
pthread_mutex_t the_mutex;
pthread_cond_t condc,condp;
int buffer=0;
void * producer(void *ptr){
    for(int i=1;i<MAX;i++){
        pthread_mutex_lock(&the_mutex); //互斥使用缓冲区
        while(buffer!=0)
            pthread_cond_wait(&condp,&the_mutex);
        buffer=i;                      //将数据放入缓冲区
        pthread_cond_signal(&condc);   //唤醒消费者
        pthread_mutex_unlock(&the_mutex);
    }
    pthread_exit(0);
}

void * consumer(void *ptr){
    for(int i=1;i<MAX;i++){
        pthread_mutex_lock(&the_mutex); //互斥使用缓冲区
        while(buffer==0)
            pthread_cond_wait(&condc,&the_mutex);
        buffer=0;                      //从缓冲区取出数据
        pthread_cond_signal(&condp);   //唤醒生产者
        pthread_mutex_unlock(&the_mutex);
    }
    pthread_exit(0);
}
int main(int argc,char **argv){
    pthread_t pro,con;
    pthread_mutex_init(&the_mutex,0);
    pthread_cond_init(&condc,0);
    pthread_cond_init(&condp,0);
    pthread_create(&con, 0, consumer, 0);
    pthread_create(&pro, 0, producer, 0);
    pthread_join(pro,0);
    pthread_join(con,0);
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
    pthread_mutex_destroy(&the_mutex);
}
