#include <stdio.h>
#include <unistd.h>
#include <string.h>
void* get_free_space_addr(pid_t pid)
{
    char filename[85];
    char line[200];
    char temp[200];
    FILE *fp;
    void *addr;
    int flag=0;
    sprintf(filename,"/proc/%d/maps",pid);
    fp=fopen(filename,"r");
    if(fp==NULL)
        return NULL;
    while(fgets(line,200,fp)!=NULL)
    {
        sscanf(line,"%lx-%*lx %*s %*s %s",&addr,temp);
        if(!strncmp(temp,"00:00",strlen("00:00")))
        {
            flag=1;
            break;
        }
    }
    if(flag)
        return addr;
    else
        return NULL;
}

void *get_lib_space(pid_t pid,const char *lib_name)
{
    char file_name[50];
    FILE *fp;
    char temp[200];
    char line[200];
    void *addr;
    int flag;
    sprintf(file_name,"/proc/%d/maps",pid);
    fp=fopen(file_name,"r");
    if(fp==NULL)
        return NULL;
    while(fgets(line,200,fp)!=NULL)
    {
        sscanf(line,"%lx-%*lx %*s %*s %*s %*s %s",&addr,temp);
        if(strstr(temp,lib_name)!=NULL)
        {   
            flag=1;
            break;
        }
    }
    if(flag)
        return addr;
    else
        return NULL;
}

