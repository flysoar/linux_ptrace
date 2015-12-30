#include <sys/ptrace.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "f_ptrace.h"

int ptrace_get_data(pid_t pid,void *local_addr,void *remote_addr,long len)
{
    union u{
        long num;
        char bytes[sizeof(long)];
    }data;
    char *addr=(char *)local_addr;
    int max=len/sizeof(long);
    int i;
    for(i=0;i<max;i++)
    {
        data.num=ptrace(PTRACE_PEEKTEXT,pid,remote_addr+i*sizeof(long),NULL);
        memcpy(addr,data.bytes,sizeof(long));
		addr+=sizeof(long);
    }
    if(len%sizeof(long))
    {
        data.num=ptrace(PTRACE_PEEKTEXT,pid,remote_addr+i*sizeof(long),NULL);
        memcpy(addr,data.bytes,len%sizeof(long));
    }
    return len;
}

int ptrace_put_data(pid_t pid,void *local_addr,void *remote_addr,long len)
{
    union u{
        long num;
        char bytes[sizeof(long)];
    }data;
    char *addr=(char *)local_addr;
    int max=len/sizeof(long);
    int i;
    for(i=0;i<max;i++)
    {
        memcpy(data.bytes,addr,sizeof(long));
		addr+=sizeof(long);
        ptrace(PTRACE_POKETEXT,pid,remote_addr+i*sizeof(long),data.num);
    }
    if(len%sizeof(long))
    {
        memcpy(data.bytes,addr,len%sizeof(long));
        ptrace(PTRACE_POKETEXT,pid,remote_addr+i*sizeof(long),data.num);
    }
    return len;
}

