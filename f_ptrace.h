/*
 * =====================================================================================
 *
 *       Filename:  f_ptrace.h
 *
 *    Description:  some function and type of this tool
 *
 *        Version:  1.0
 *        Created:  12/30/2015 12:18:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  flysoar
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdlib.h>

typedef union data{
	long val;
	char chars[sizeof(long)];
}data;

void remote_push(pid_t pid,data datas);
void *remote_call(pid_t pid,void *remote_addr,data* args,int argc);

int ptrace_get_data(pid_t pid,void *local_addr,void *remote_addr,long len);
int ptrace_put_data(pid_t pid,void *local_addr,void *remote_addr,long len);

void* get_free_space_addr(pid_t pid);
void* get_lib_space(pid_t pid,const char *lib_name);
void* get_remote_func_addr(pid_t,void *local_function,const char* lib_name);

int intercept_write(pid_t pid,const char* target);
char* intercep_read(pid_t pid,long *len);
