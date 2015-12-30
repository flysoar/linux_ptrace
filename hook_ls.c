/*
 * =====================================================================================
 *
 *       Filename:  hook_ls.c
 *
 *    Description:  make the ls can't output some string but it's just a test
 *
 *        Version:  1.0
 *        Created:  12/30/2015 05:40:35 PM
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
#include <sys/ptrace.h>
#include <sys/user.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "f_ptrace.h"

#define PATH "/bin/ls"
#define TARGET "test"

int main(int argc,char *argv[])
{
	pid_t pid=fork();
	if(pid<0)
		exit(1);
	else if(pid==0)
	{
		ptrace(PTRACE_TRACEME,0,NULL,NULL);
		execv(PATH,argv);
		exit(1);
	}
	int stat;
	while(intercept_write(pid,TARGET)!=-1)
		ptrace(PTRACE_SYSCALL,pid,NULL,NULL);
	return 0;
}
