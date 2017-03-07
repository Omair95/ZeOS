/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <system.h>

#include <interrupt.h>
#define LECTURA 0
#define ESCRIPTURA 1


int check_fd(int fd, int permissions)
{
  if (fd != 1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{
  int PID=-1;

  // creates the child process
  
  return PID;
}

void sys_exit()
{  
}

int sys_write(int fd, char * buffer, int size) 
{
	int f = check_fd(fd, ESCRIPTURA);
	if (f < 0) return f;
	if (buffer == NULL) return -22;
	if (size <= 0) return -22;
	
	char sys_buff[4];
	int ret = 0;
	while (size > 4) {
		copy_from_user(buffer, sys_buff, 4);
		ret += sys_write_console(sys_buff, 4);
		size = size - 4;
		buffer += 4;
	}
	copy_from_user(buffer, sys_buff, size);
	ret += sys_write_console(sys_buff, size);
	return ret;
   
}

int zeos_ticks;

int sys_gettime() {
   return zeos_ticks;
}



