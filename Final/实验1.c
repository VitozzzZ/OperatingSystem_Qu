#include <iostream>
#include <cstdio>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#define N 6
#define LEFT i
#define RIGHT (i+1) % N
using namespace std;

class Semaphore {
	private:
		sem_t sem;
	public:
		Semaphore(int value = 1) {
			sem_init(&sem,0,value); //初始化信号量,0表示信号量在线程间共享
		}
		void P() {
			sem_wait(&sem); //等待信号，获取拥有权
		}
		void V() {
			sem_post(&sem); //释放信号，释放拥有权
		}
};

Semaphore mutex[N];
pthread_t thread[N];

int id[N];
void* solve(void* param) {
	int i = *((int*)param);
	while(1) {
		if(i % 2 == 0) {
			mutex[LEFT].P();
			mutex[RIGHT].P();
			printf("哲学家%d就餐\n",i+1);
			mutex[RIGHT].V();
			mutex[LEFT].V();
		}else {
			mutex[RIGHT].P();
			mutex[LEFT].P();
			printf("哲学家%d就餐\n",i+1);
			mutex[LEFT].V();
			mutex[RIGHT].V();
		}
		sleep(1); //线程等待
	}
	pthread_exit(NULL); //结束线程
}
void thread_create() { //进程创建
	int tmp;
	for(int i = 0; i < N; i++) {
		tmp = pthread_create(&thread[i],NULL,solve,&id[i]);
		if(tmp != 0) {
			printf("线程%d创建失败\n",i);
		}
	}
}
void thread_wait() {
	for(int i = 0; i < N; i++) {
		pthread_join(thread[i],NULL);
		printf("线程%d结束\n",i);
	}
}
int main() {
	for(int i = 0; i < N; i++) {
		id[i] = i;
	}
	thread_create();
	thread_wait();
	return 0;
}
