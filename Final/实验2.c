#include<iostream>
#include<fstream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
using namespace std;
struct student
{
  int num;
  char name[20];
  char sex[10];
  int age;

};

void step1()
{
 ofstream outfile("/home/yy/hw2.txt",ios::out);
 cout<<"write the information of ten students"<<endl; 
for(int i=1;i<=10;i++)
   {
     cin>>rec.num>>rec.name>>rec.sex>>rec.age;
     outfile.write((const char*)(&rec),sizeof(rec));
   }
outfile.close();
 cout<<endl<<endl;
 ifstream infile("/home/yy/hw2.txt",ios::in);
cout<<"show the information of the students"<<endl;
 while(!(infile.peek()==EOF))
 {
  infile.read((char*)(&read),sizeof(read));
  cout<<read.num<<" "<<read.name<<" "<<read.sex<<" "<<read.age<<endl;
 }
}
void step2()
{
 fstream ffile;
 ffile.open("/home/yy/hw2.txt",ios::binary | ios::out | ios::in); 
struct student rec,read;
  int t;
  int x=sizeof(rec);
 t=sizeof(rec)*3;
ffile.seekg(0,ios::end);
streampos ps=ffile.tellg();
int len=ps;
int i=0;
while(t<len)
{
 ffile.seekg(t,ios::beg);
 ffile.read((char*)(&rec),sizeof(rec));
 t=t-sizeof(rec);
 ffile.seekg(t,ios::beg);
 ffile.write((const char*)(&rec),sizeof(rec));
 t=t+sizeof(rec)*2;
 
}
cout<<"show the information of the students"<<endl;
ffile.seekg(x,ios::beg);
  while(!(ffile.peek()==EOF))
 {
  ffile.seekg(-x,ios::cur);
  ffile.read((char*)(&read),sizeof(read));
  cout<<read.num<<" "<<read.name<<" "<<read.sex<<" "<<read.age<<endl;
  ffile.seekg(x,ios::cur);
 }
}

void step3()
{
 struct student a={9,"baka","female",9};
 fstream ffile;
 ffile.open("/home/yy/hw2.txt",ios::binary | ios::out | ios::in);
struct student rec,read;
  int t;
  int x=sizeof(rec);
 t=sizeof(rec)*3;
ffile.seekg(0,ios::end);
streampos ps=ffile.tellg();
int len=ps;
while(t+3*sizeof(rec)<=len)
{
 
 ffile.seekg(-t,ios::end);
 ffile.read((char*)(&rec),sizeof(rec));
 ffile.seekg(0,ios::cur);
 ffile.write((const char*)(&rec),sizeof(rec));
 t=t+sizeof(rec);
}

ffile.seekg(160,ios::beg);
ffile.write((const char*)(&a),sizeof(rec));
cout<<"show the information of the students"<<endl;
ffile.seekg(0,ios::beg);
  while(!(ffile.peek()==EOF))
 {
  ffile.read((char*)(&read),sizeof(read));
  cout<<read.num<<" "<<read.name<<" "<<read.sex<<" "<<read.age<<endl;
 }

}
int main()
{
 cout<<"step1 going"<<endl<<endl;
 step1();
 cout<<"step2 going"<<endl<<endl;
 step2();
 cout<<"step3 going"<<endl<<endl;
 step3();
}
