#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int counter=0; /*两个线程都能访问的共享变量 */

void  thread1(void *arg);
void  thread2(void *arg);
pthread_mutexattr_t attr;
pthread_mutex_t mutex;
int err = 0;

int  main(int argc, char *argv[])
{
	pthread_t id1,id2;
	if(err = pthread_mutexattr_init(&attr) != 0){//初始化锁属性
		printf("init attr failed!\n");
		exit(1);
	}
	if(err = pthread_mutex_init(&mutex,&attr) != 0){//初始化锁
		printf("init mutex failed!\n");
		exit(1);
	}

	/*创建第1个线程*/
	pthread_create(&id1,NULL,(void *)thread1, NULL);
	/*创建第2个线程*/
	pthread_create(&id2,NULL,(void *)thread2, NULL); 

	pthread_join(id1,NULL); /*等待第1个线程结束*/
	pthread_join(id2,NULL); /*等待第2个线程结束*/
	  /*打印counter最后的值*/
	printf("最后的counter值为%d\n",counter);

	exit(0);
}

void  thread1(void *arg) /*第1个线程执行代码*/
{
	int i,val;
	for(i=1;i<=5;i++){
		pthread_mutex_lock(&mutex);//加锁
		val=++counter;
		printf("第1个线程：第%d次循环，第1次引用counter=%d\n",i,counter);  /*LINE A*/
		usleep(300);   /*睡眠或挂起300毫秒钟*/
		printf("第1个线程：第%d次循环，第2次引用counter=%d\n",i,counter);  /*LINE B*/
		counter=val;
		pthread_mutex_unlock(&mutex);//解锁
	}
}

void thread2(void *arg) /*第2个线程执行代码*/
{
	int i,val;
	for(i=1;i<=5;i++){
		pthread_mutex_lock(&mutex);//加锁
		val=++counter; 
		usleep(100);  /*睡眠或挂起100毫秒钟*/
		printf("第2个线程：第%d次循环，counter=%d\n",i,counter);
		counter=val; 
		pthread_mutex_unlock(&mutex);//解锁
	}
}
