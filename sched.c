/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>


/**
 * Container for the Task array and 2 additional pages (the first and the last one)
 * to protect against out of bound accesses.
 */
union task_union protected_tasks[NR_TASKS+2]
  __attribute__((__section__(".data.task")));

union task_union *task = &protected_tasks[1]; /* == union task_union task[NR_TASKS] */

struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  //return list_entry( l, struct task_struct, list);
  int dir = &l;
  return (struct task_struct*)(dir&0xfffff000);
}

struct task_struct * idle_task;

extern struct list_head blocked;


/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t) 
{
	return t->dir_pages_baseAddr;
}

/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

void init_idle (void)
{
  idle_task = list_head_to_task_struct(list_first(&freequeue));
  //list_del(list_first(&freequeue));
  union task_union *idle_union = (union task_union*) idle_task;

  idle_task->PID = 0;
  allocate_DIR(idle_task);
  //Punto 4: Inicialitzar contexto para el proceso para restaurarlo cuando se le asigna cpu.
  idle_union->stack[1023] = (unsigned long)&cpu_idle;
  idle_union->stack[1022] = 0;
  idle_union->task.initial_ebp = &idle_union->stack[1022];
  sys_write(1, "HOLA", 4);
  cpu_idle();
}

void init_task1(void)
{
   struct task_struct *init = list_head_to_task_struct(list_first(&freequeue));
   //list_del(list_first(&freequeue));
   union task_union *init_union = (union task_union*) init;
   init->PID = 1;
   allocate_DIR	(init);
   set_user_pages(&init_union->task);
   TSS.esp0 = init_union->stack[1024]; //Update TSS
   page_table_entry * dir_task1 = get_DIR(&init_union->task);	
   set_cr3(dir_task1);
}


void init_sched(){
  INIT_LIST_HEAD(&freequeue);
    
  int i;
  for(i = 0; i < NR_TASKS; i++) {
	list_add(&task[i].task.list, &freequeue);
  }
  INIT_LIST_HEAD(&readyqueue);
}

struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}


void inner_task_switch(union task_union *new) {

}


void task_switch(union task_union *new) {

  inner_task_switch(new);

}