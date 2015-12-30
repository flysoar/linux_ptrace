/*
 * =====================================================================================
 *
 *       Filename:  do_write_read.c
 *
 *    Description:  hook the read and write by ptrace in x86_64
 *
 *        Version:  1.0
 *        Created:  12/30/2015 10:55:20 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  flysoar
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/user.h>
#include <sys/ptrace.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/wait.h>

#include "f_ptrace.h"

int intercept_write(pid_t pid,const char* target)
{
	struct user_regs_struct regs;
	int stat;
	unsigned long long int orig_len;
	int in_syscall=0;
	while (1)
	{
		ptrace(pid,PTRACE_SYSCALL,NULL,NULL);
		waitpid(pid,&stat,0);
		if(WIFEXITED(stat))
			return -1;
		ptrace(PTRACE_GETREGS,pid,NULL,&regs);
		if(regs.orig_rax==SYS_write)
		{
			if(in_syscall==0)
			{
				in_syscall=1;
				orig_len=regs.rdx;
				char *temp=malloc(orig_len);
				if(temp==NULL)
					return -1;
				ptrace_get_data(pid,temp,(void *)regs.rsi,orig_len);
				if(strstr(temp,target)!=NULL)
					regs.rdx=0;
				ptrace(pid,PTRACE_SETREGS,NULL,&regs);
			}
			else
			{
				ptrace(pid,PTRACE_GETREGS,NULL,&regs);
				regs.rax=orig_len;
				ptrace(pid,PTRACE_SETREGS,NULL,&regs);
				return 0;
			}
		}
	}
}

int intercept_read(pid_t pid,long* len,char** ptr)
{
	int in_syscall=0;
	struct user_regs_struct regs;
	unsigned long long int orig_len;
	int stat;
	while(1)
	{
		ptrace(PTRACE_SYSCALL,pid,NULL,NULL);
		waitpid(pid,&stat,0);
		if(WIFEXITED(stat))
			return -1;
		ptrace(PTRACE_GETREGS,pid,NULL,&regs);
		if(regs.orig_rax==SYS_read)
		{
			if(in_syscall==0)
			{
				in_syscall=1;
			}
			else
			{
				if(regs.rdx<=0)
					return -2;
				*ptr=malloc(regs.rdx);
				if(*ptr==NULL)
					return -3;
				ptrace_get_data(pid,*ptr,(void *)regs.rsi,regs.rdx);
				return 0;
			}
		}
	}
}
