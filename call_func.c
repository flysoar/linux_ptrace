#include <sys/user.h>
#include <sys/ptrace.h>
#include <stdlib.h>
#include <string.h>

#include "f_ptrace.h"

#define BAD_ADDR 0x2333333333333333

void remote_push(pid_t pid,data datas)
{
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS,pid,NULL,&regs);
    void *rsp=regs.rsp;
    rsp-=sizeof(long);
    regs.rsp=rsp;
    ptrace(PTRACE_POKETEXT,pid,rsp,&datas.val);
    ptrace(PTRACE_SETREGS,pid,NULL,&regs);
}

void *remote_call(pid_t pid,void *remote_addr,data* args,int argc)
{
    int temp;
    if(argc>6)
        temp=6;
    else
        temp=argc;
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS,pid,NULL,&regs);
    switch(temp)
    {
        case 6:
            regs.r9=args[5].val;
        case 5:
            regs.r8=args[4].val;
        case 4:
            regs.rcx=args[3].val;
        case 3:
            regs.rdx=args[2].val;
        case 2:
            regs.rsi=args[1].val;
        case 1:
            regs.rdi=args[0].val;
    }
    if(argc>6)
        for(int i=0;i<argc-6;i++)
            remote_push(pid,args[i+6]);
    remote_push(pid,(data)BAD_ADDR);
    void *ret=regs.rip;
    regs.rip=remote_addr;
    ptrace(PTRACE_SETREGS,pid,NULL,&regs);
    return ret;
}
      
