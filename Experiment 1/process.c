#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>  
#include <linux/list.h>

int init_module()
{
	struct task_struct  *p;
	int totalCount =0,
		runningCount=0,
		interruptibleCount=0,
		uninterruptibleCount=0,
		zombieCount=0,
		stoppedCount=0,
		swappingCount=0;
	printk(KERN_ALERT "name\tpid\tstate\tparent\n");
	for (p = &init_task; (p = next_task(p)) != &init_task;) 
	{
		totalCount++;
		switch(p->state)
		{
			case TASK_RUNNING:
				runningCount++;
				printk( KERN_ALERT "%s\t%u\tTASK_RUNNING\t%u\n",p->comm,p->pid,p->parent->pid);
				break;
			case TASK_INTERRUPTIBLE:
				interruptibleCount++;
				printk( KERN_ALERT "%s\t%u\tTASK_INTERRUPTIBLE\t%u\n",p->comm,p->pid,p->parent->pid);
				break;
			case TASK_UNINTERRUPTIBLE:
				uninterruptibleCount++;
				printk( KERN_ALERT "%s\t%u\tTASK_UNINTERRUPTIBLE\t%u\n",p->comm,p->pid,p->parent->pid);
				break;
			case EXIT_ZOMBIE:
				zombieCount++;
				printk( KERN_ALERT "%s\t%u\tTASK_ZOMBIE\t%u\n",p->comm,p->pid,p->parent->pid);
				break;
			case TASK_STOPPED:
				stoppedCount++;
				printk( KERN_ALERT "%s\t%u\tTASK_STOPPED\t%u\n",p->comm,p->pid,p->parent->pid);
				break;
			default:
				swappingCount++;
				printk( KERN_ALERT "%s\t%u\tTASK_SWAPPING\t%u\n",p->comm,p->pid,p->parent->pid);
		}
	}
	printk( KERN_ALERT "Total:%d ", totalCount);
	printk( KERN_ALERT "RUNNING:%d ", runningCount);
	printk( KERN_ALERT "INTERRUPTIBLE:%d ", interruptibleCount);
	printk( KERN_ALERT "UNINTERRUPTIBLE:%d ", uninterruptibleCount);
	printk( KERN_ALERT "ZOMBIE:%d ", zombieCount);
	printk( KERN_ALERT "STOPPED:%d\n", stoppedCount);
	printk( KERN_ALERT "SWAPPING:%d\n", swappingCount);
	return 0;
}

void cleanup_module()
{
	printk( KERN_ALERT "exit success\n");
}
