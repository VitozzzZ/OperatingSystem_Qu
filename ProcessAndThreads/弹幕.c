#include <iostream>
#include <string>
#include <windows.h>
#include<stdio.h>
using namespace std; 
int main() 
{
	void PrintChar(char *ch,UINT count,UINT x,UINT y);
	double x=6,y=1;
	int a;a=100;
	while(y)
	{
	PrintChar("Hello",6,y,x);
	Sleep(a);
	PrintChar("     ",6,y,x);	
	y=y+1;
	if(y>100)
	{
		y=1;
	}
	}

	return 0;
}
void PrintChar(char *ch,UINT count,UINT x,UINT y)  //在坐标(x,y)处输出字符串ch,ch里有count个字符
{
 HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
 COORD pos;
 ULONG unuse;
 pos.X=x;
 pos.Y=y;
 CONSOLE_SCREEN_BUFFER_INFO bInfo; // 窗口缓冲区信息
 GetConsoleScreenBufferInfo(h, &bInfo );
 WriteConsoleOutputCharacterA(h,ch,count,pos,&unuse);
}

2.

#include <windows.h>
//#include <conio.h>
#include<stdlib.h >
#include <iostream>
using namespace std; 
void gotoxy(int x, int y)
{
    COORD coord = {x, y};   
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
int main()
{
	for(int i=0;;i++)
	{
		while(i>100)
	    {
	    	i=0;
		}
		gotoxy(i,2);
		cout<<"hello";
		Sleep(100);
		system("cls");
		
	   
	}
	return 0;
	
	
}
