/*
 * =====================================================================================
 *
 *       Filename:  ptrace_break.c
 *
 *    Description:  add the breakpoint
 *
 *        Version:  1.0
 *        Created:  01/01/2016 08:12:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  flysoar
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>

#include "f_ptrace.h"

#define INT3 0xcc

void ptrace_make_break(pid_t pid,void *addr,data* s_data)
{
	data temp;
	data ret=*s_data;
	temp.chars[0]=INT3;
	ret.val=ptrace(PTRACE_PEEKTEXT,pid,addr,NULL);
	ptrace(PTRACE_POKETEXT,pid,addr,temp.val);
}

void ptrace_break_rec(pid_t pid,void *addr,data* s_data)
{
	data temp=*s_data;
	ptrace(PTRACE_POKEDATA,pid,addr,temp.val);
}

