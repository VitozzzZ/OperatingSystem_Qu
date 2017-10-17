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
