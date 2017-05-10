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

void stats_work(int pid) {
	char buffer[20];
	struct stats stats;
	if (get_stats(pid,&stats) == -1) perror();
	else {

		write(1, "\n--User Stats---", 16);
		itoa(pid, buffer);
		write(1, buffer, strlen(buffer));
		write(1, "--", 2);		

		write(1, "\nUser ticks: ", 13);
		itoa(stats.user_ticks, buffer);
		write(1, buffer, strlen(buffer));
		write(1, "--", 2);

		write(1,"\nSystem ticks: ", 15);
		itoa(stats.system_ticks, buffer);
		write(1, buffer, strlen(buffer));
		write(1, "----", 4);

		write(1, "\nBlocked ticks: ", 16);
		itoa(stats.blocked_ticks, buffer);
		write(1, buffer, strlen(buffer));
		write(1, "--", 2);

		write(1, "\nReady ticks: ", 14);
		itoa(stats.ready_ticks, buffer);
		write(1, buffer, strlen(buffer));
		write(1, "--", 2);

		write(1,"\n------------------\n",20);
	}
}


void fork_works() {
	char buffer[5];
	int pid;

	//write(1, "\nabans del fork\n", 20);
	pid = fork();
	//write(1, "\ndespres del fork\n", 20);

	set_sched_policy(0);

	if (pid > 0) { 		//Parent process
		write(1, "\nPare\n", 8);
		
		pid = getpid();
		itoa(pid, buffer);
		if (write(1, "\nParent PID: ", 13) == -1) perror();
		if (write(1, buffer, strlen(buffer))  == -1) perror();
		//read(0, buffer, 5000);
		while(1) {
			if (gettime()%10 == 0) {
				if (write(1, "\nParent PID: ", 13) == -1) perror();
				if (write(1, buffer, strlen(buffer))  == -1) perror();
				stats_work(1);
			}

		}
		write(1, "\nParent finish\n", 15);
		//exit();
	} 
	else if (pid == 0) { 	//Child process
		write(1, "\nDins FILL\n", 10);
		write(1, "\nFill\n", 5);
		pid = getpid();
		itoa(pid, buffer);
		if (write(1, "\nChild PID: ", 12) == -1) perror();
		if (write(1, buffer, strlen(buffer))  == -1) perror();

		while(1) {
			if (gettime()%10 == 0) {
				if (write(1, "\nChild PID: ", 12) == -1) perror();
				if (write(1, buffer, strlen(buffer))  == -1) perror();
				stats_work(1001);
			}
		}
		write(1, "\nChild finish\n", 15);
		exit();
	} else if (pid == -1) {
		write(1, "\nError", 6);
		perror();
	}

}


//Workload 1: Uso de CPU
void W1() 
{
	int fork1 = fork();
	int i, j = 0;
	if (fork1 > 0) 
	{
		for (i = 0; i < 1000000; i++)
		{
			gettime();
			j++;
		}
		//stats_work(getpid());
	}
	else if (fork1 == 0)
	{
		for (i = 0; i < 1000000; i++)
		{
			gettime();
			j++;
		}
		stats_work(getpid());
		exit();
	}
	
	int fork2 = fork();
	if (fork2 > 0) 
	{
		stats_work(getpid());
		exit();
	}
	else if (fork2 == 0)
	{
		for (i = 0; i < 1000000; i++)
		{
			gettime();
			j++;
		}
		stats_work(getpid());
		exit();
	}

}


//Workload 2: Bloqueos cortos
void W2() 
{
	int fork1 = fork();
	int i, j = 0;
	char * buffer;
	if (fork1 > 0) 
	{
		for (i = 0; i < 1000; i++)
		{
			read(0, buffer, 10);
			j++;
		}
		//stats_work(getpid());
	}
	else if (fork1 == 0)
	{
		for (i = 0; i < 1000; i++)
		{
			read(0, buffer, 10);
			j++;
		}
		stats_work(getpid());
		exit();
	}
	
	int fork2 = fork();
	if (fork2 > 0) 
	{
		read(0, buffer, 10);
		stats_work(getpid());
		exit();
	}
	else if (fork2 == 0)
	{
		for (i = 0; i < 1000; i++)
		{
			read(0, buffer, 10);
			j++;
		}
		stats_work(getpid());
		exit();
	}
}

//Workload 3: Bloqueo largo + Uso CPU
void W3() 
{
	int fork1 = fork();
	int i, j = 0;
	char * buffer;
	if (fork1 > 0) 
	{
		read(0, buffer, 1000);
		for (i = 0; i < 100000; i++)
		{
			gettime();
			j++;
		}
		//stats_work(getpid());
	}
	else if (fork1 == 0)
	{
		read(0, buffer, 1000);
		for (i = 0; i < 100000; i++)
		{
			gettime();
			j++;
		}
		stats_work(getpid());
		exit();
	}
	
	int fork2 = fork();
	if (fork2 > 0) 
	{
		read(0, buffer, 1000);
		stats_work(getpid());
		exit();
	}
	else if (fork2 == 0)
	{
		read(0, buffer, 1000);
		for (i = 0; i < 100000; i++)
		{
			gettime();
			j++;
		}
		stats_work(getpid());
		exit();
	}

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
	
	int time;
	char timestring[25];

	set_sched_policy(1);
	W3();

	while(1) {
		
	}
}
