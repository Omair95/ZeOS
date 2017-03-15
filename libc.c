/*
 * libc.c 
 */

#include <libc.h>

#include <errno.h>

#include <types.h>

int errno;

void itoa(int a, char *b)
{
  int i, i1;
  char c;
  
  if (a==0) { b[0]='0'; b[1]=0; return ;}
  
  i=0;
  while (a>0)
  {
    b[i]=(a%10)+'0';
    a=a/10;
    i++;
  }
  
  for (i1=0; i1<i/2; i1++)
  {
    c=b[i1];
    b[i1]=b[i-i1-1];
    b[i-i1-1]=c;
  }
  b[i]=0;
}

int strlen(char *a)
{
  int i;
  
  i=0;
  
  while (a[i]!=0) i++;
  
  return i;
}


int write(int fd, char * buffer, int size)
{
	int ret;
	__asm__("int $0x80 \n\t"
		: "=a" (ret)
		: "b" (fd), "c" (buffer), "d" (size), "a" (4));
	if (ret < 0) {
		errno = -ret;
		return -1;
	}
	errno = 0;
	return ret;
}

int gettime() {
	int ret;
	__asm__("int $0x80 \n\t"
		: "=a" (ret)
		: "a" (10));
	if (ret < 0) {
		errno = -ret;
		return -1;
	}
	errno = 0;
	return ret;
}

void perror() {
	char * descr;
	if (errno < 0) {
		switch(errno) {
			case -EPERM: descr = "-1"; break;
			case -ENOENT: descr = "-1"; break;
			case -EBADF: descr = "Bad file descriptor"; break;
			case -EACCES: descr = "Permission denied"; break;
			case -EINVAL: descr = "Invalid argument"; break;
			case -ENOSYS: descr = "Function not implemented"; break;
			default: descr = "Unknown error"; break;
	 	}	
		write(1, descr, strlen(descr));
		write(1, "\n", 1);
	}
}

int getpid(void) 
{
	
}

int fork(void)
{

}



