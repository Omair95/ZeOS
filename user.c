#include <libc.h>

char buff[24];

int pid;

int add(int par1, int par2)
{
    int result;
    result = 0;
    __asm__("add %1, %2; \n\t"
	    "mov %1, %0; \n\t"
	: "=r"(result)
	: "a"(par1), "b"(par2) );
    return result;
}

  
long inner(long n)
{
   int i;
   long suma;
   suma = 0;
   for (i = 0; i < n; ++i) suma += i;
   return suma;
}

long outer(long n)
{
   int i;
   long acum;
   acum = 0;
   for (i = 0; i < n; ++i) acum = acum + inner(i);
   return acum;
}

int __attribute__ ((__section__(".text.main")))
  main(void)
{
    /* Next line, tries to move value 0 to CR3 register. This register is a privileged one, and so it will raise an exception */
     /* __asm__ __volatile__ ("mov %0, %%cr3"::"r" (0) ); */

  /*long count, acum;
  count = 75;
  acum = 0;
  acum = outer(count); 
  int result = add(2,3); */

  int w = write(1, "Hello", 8);
  if (w == -1) perror("Error en write");

  int time;
  char timestring[25];
  
  while(1) {
    write(1, "\n", 1);
    time = gettime();
    w = write(1, "Time: ", 6);
    itoa(time,timestring);
    w = write(1, timestring, strlen(timestring));
    if (w == -1) perror("Error en write");
  }
}
