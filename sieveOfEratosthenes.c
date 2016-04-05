#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_cond_t cond;
pthread_mutex_t mutex;
int work = 0;

typedef struct ready_queue{
	pthread_t id;
	struct ready_queue *next;
};

struct ready_queue *head;
struct ready_queue *tail;

void *slaveLogic(void *arg){
	int ready;
	pthread_mutex_lock(&mutex);
	//while(!work)
		//pthread_cond_wait();

	pthread_mutex_unlock(&mutex);
	return 0;
}

int main(int argc, char *argv[]){
	int num_threads, n, c;

	/* Dealing with command line inputs */
	if(argc != 4){
		printf("Usage ./%s -p -n -c\n",argv[0]);
	 	return 1;	
	}
	else{
		num_threads = atoi(argv[1]);
		n=atoi(argv[2]);
		c=atoi(argv[3]);
	}

	/* Initialize the queue*/
	head = malloc(sizeof(struct ready_queue));
	pthread_t slaves[num_threads];
	int i;
	struct ready_queue *current = head;
	pthread_create(&slaves[0],NULL,slaveLogic,NULL);
	current->id = slaves[0];
	tail = head;
	
	for(i=1;i<num_threads;i++){
		pthread_create(&slaves[i],NULL,slaveLogic,(void*)current);
		current = malloc(sizeof(struct ready_queue));
		current->id = slaves[i];
		current->next = head;
		head = current;
	}
		





	for(i=0;i<num_threads;i++){
		pthread_join(slaves[i],NULL);
	}
	
	return 0;
}
