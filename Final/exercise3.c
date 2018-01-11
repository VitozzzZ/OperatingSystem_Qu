#include <dirent.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
using namespace std;                                                                             
int readFileList(char *basePath)
{
       DIR *dir;
       struct dirent *ptr;
       char base[1000];
       if((dir=opendir(basePath))==NULL)
        {
         cout<<"open dir error!"<<endl;
        }
       while((ptr=readdir(dir))!=NULL)
        {
          if(strcmp(ptr->d_name,".")==0||strcmp(ptr->d_name,"..")==0)
          {continue;}
           else
            {
            if(ptr->d_type==8)
            {cout<<"d_name"<<basePath<<ptr->d_name<<endl;}
            else
              { 
             if(ptr->d_type==10)
             {cout<<"d_name"<<basePath<<ptr->d_name<<endl;}
            else
               {
              if(ptr->d_type==4)
              {memset(base,'\0',sizeof(base));
               strcpy(base,basePath);
               strcat(base,"/");
               strcat(base,ptr->d_name);
               readFileList(base);
                 }
               }
               }
            }
         }
closedir(dir);
return 1;
}
int main()
{
char basePath[1000];
cout<<"the current dir is";
cin>>basePath;
readFileList(basePath);
return 0;
}
