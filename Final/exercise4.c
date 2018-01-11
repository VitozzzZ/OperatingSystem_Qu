#include <iostream>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#define NUM_THREADS 2 
using namespace std;
void distribution(int m,int*s)    
{
	int i,j;
	bool flag=true;
	for(int j=0;j<30;j++)
	{
		if(s[j]!=0)
		flag==false;
	}
	while(i<30&&flag==true)
	{
	    srand((unsigned)time(NULL)); //initialize random number seeds
		i=rand()%30;//i take radom numbers
		if(s[i]==0)
		{
			s[i]=1;           
			cout<<"NO."<<m<<""<<"  get "<<"NO. "<<i+1<<"ram"<<endl;
			flag=true;
			break;
		}
	}
	if(flag==false)
	{
		cout<<"NO ram left!"<<endl; 
	}
	s[i]=0;
	sleep(5);
	cout<<"realse NO. "<<i+1<<"ram"<<endl;	
}
int main()
{
	char *n;
    double c;
	n=(char *)malloc(30*1024);
	if(!n)
	{
		cout<<"malloc ram error!!!"<<endl;
		return -1;
	}
	int shz[30];
	memset(shz,0,30);
	pid_t pid1,pid2;//two sub-process
	while(1)
	{
		
		pid1=fork();
		if(pid1==0)
	 	{
			cout<<"process 1 request assignment"<<endl; 
			return 0;
		}
		else if(pid1>0) 
		{
			wait(NULL);
			distribution(1,shz);
		} 
		pid2=fork();
		if(pid2==0)
		{
			cout<<"process 2 request assignment"<<endl; 
			return 0;
		}
		else if(pid2>0) 
		{
			wait(NULL);
			distribution(2,shz);
		} 
	}
	free(n);
	return 0;
}
