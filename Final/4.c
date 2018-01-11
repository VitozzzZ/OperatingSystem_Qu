#include<stdio.h>  
#include<stdlib.h>  
#include<unistd.h>  
#include<pthread.h>  
#include<semaphore.h>  
  
#define doctor_NUM 3 //the number of doctor   
#define laboratorian_NUM 3 //the number of laboratorian
#define POOL_SIZE    11  
int pool[POOL_SIZE];    // buffer
int head=0; 
int rear=0; 
sem_t   room_sem;        
sem_t   doctor_sem;      
pthread_mutex_t mutex;  

//doctor
void *doctor_fun(void *arg)  
{  
    while (1)  
    {  
        sleep(5);  
        sem_wait(&room_sem);  
        pthread_mutex_lock(&mutex);  
        //doctor operate  
        pool[rear] = 1;  
        rear = (rear + 1) % POOL_SIZE;  
        printf("Doctor treat a patient. There are %d patient waiting to analysis\n", (rear-head+POOL_SIZE)%POOL_SIZE);  
        pthread_mutex_unlock(&mutex);  
        sem_post(&doctor_sem);  
    }  
}  
  
  
void *laboratorian_fun(void *arg)  
{  
    while (1)  
    {  
        int data;  
        sleep(5);  
        sem_wait(&doctor_sem);  
        pthread_mutex_lock(&mutex);  
        // laboratorian operate
        data = pool[head];  
        head = (head + 1) % POOL_SIZE;  
        printf("laboratorian analysis a patient. There are %d patient waiting to analysis\n", (rear-head+POOL_SIZE)%POOL_SIZE);  
        pthread_mutex_unlock(&mutex);  
        sem_post(&room_sem);  
    }  
}  
  
int main()  
{  
    pthread_t doctor_id[doctor_NUM];  
    pthread_t laboratorian_id[laboratorian_NUM];  
    pthread_mutex_init(&mutex, NULL);   
int ret = sem_init(&room_sem, 0, POOL_SIZE-1);  
    if (ret != 0)  
    {  
        printf("sem_init error");  
        exit(0);  
    }  
ret = sem_init(&doctor_sem, 0, 0); 
    if (ret != 0)  
    {  
        printf("sem_init error");  
        exit(0);  
    }  
    for (int i = 0; i < doctor_NUM; i++)  
    {  
        //doctor
        ret =pthread_create(&doctor_id[i], NULL, doctor_fun, NULL);  
        if (ret != 0)  
        {  
            printf("doctor_id error");  
            exit(0);  
        }  
       // laboratorian
        ret = pthread_create(&laboratorian_id[i], NULL, laboratorian_fun, NULL);  
        if (ret != 0)  
        {  
            printf("laboratorian_id error");  
            exit(0);  
        }  
    }  
    for(int i=0;i<doctor_NUM;i++)  
    {  
        pthread_join(doctor_id[i],NULL);  
        pthread_join(laboratorian_id[i],NULL);  
    }  
  
    exit(0);  
}
