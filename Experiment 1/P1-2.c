#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>

time_t endTime;
// three semaphors
sem_t mutex, students, tas;

// queue to store the students
struct
{
	pid_t pid[5];
	int count;
} queue;

pid_t pop()
{
	pid_t p = queue.pid[0];
	queue.count = queue.count - 1;
	int i;
	for (i = 0; i < queue.count; ++i)
	{
		queue.pid[i] = queue.pid[i+1];
	}	
	return p;
}

void push(pid_t p)
{
	queue.pid[queue.count] = p;
	queue.count ++;
}

pid_t gettid()
{
	return (pid_t)pthread_self();
}

void teach()
{
	if (queue.count > 0)
	{
		printf("TA: I'm helping student (tid %u)\n", pop());
	}
	else
	{
		printf("TA: I'm sleeping\n");
	}
	usleep(50);
}

void ta()
{
	while(time(NULL) < endTime)
	{
		// P(students)
		sem_wait(&students);
		// P(mutex)
		sem_wait(&mutex);
		teach();
		// V(mutex)
		sem_post(&mutex);
		// V(tas)
		sem_post(&tas);
	}
}

void student()
{
		sem_wait(&mutex);
		// queue
		if (queue.count < 5)
		{
			printf("Student (tid %u): I'm in line!\n", gettid());
			push(gettid());
			// V(mutex)
			sem_post(&mutex);
			// V(students)
			sem_post(&students);
			// P(tas)
			sem_wait(&tas);
		}
		// go back
		else
		{
			printf("Student(tid %u): I'll go back and wait.\n", gettid());
			// V(mutex)
			sem_post(&mutex);
			// V(students)
			sem_post(&students);
			// P(tas)
			sem_wait(&tas);
		}
}

int main(int argc, char const *argv[])
{
	pthread_t id1;
	pthread_t id2[20];
	endTime = time(NULL) + 5;

	// initialize semaphors
	sem_init(&mutex, 0, 1);
	sem_init(&students, 0, 0);
	sem_init(&tas, 0, 1);

	// create ta
	pthread_create(&id1, NULL, (void *)ta, NULL);
	int i;
	// create students
	for (i = 0; i < 20; ++i)
	{
		pthread_create(&id2[i], NULL, (void *)student, NULL);
	}

	// wait students
	for (i = 0; i < 20; ++i)
	{
		pthread_join(id2[i], NULL);
	}
	// wait ta
	pthread_join(id1, NULL);
	return 0;
}