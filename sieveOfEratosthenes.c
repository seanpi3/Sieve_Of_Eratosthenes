#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_cond_t cond;
pthread_mutex_t mutex;
pthread_mutex_t ready_queue_mutex;
int work = 0;

typedef struct ready_queue{
	pthread_t id;
	struct ready_queue *next;
};

struct ready_queue *head;
struct ready_queue *tail;

typedef struct numbers{
	int number;
	int marked;
};
struct numbers** number_array;

/*Logic for the slave threads*/
void *slaveLogic(void *arg){
	int ready;
	pthread_t id = (pthread_t)arg;

	/*Add self to the ready queue*/
	pthread_mutex_lock(&ready_queue_mutex);
	tail->next = malloc(sizeof(struct ready_queue));
	tail = tail->next;
	tail->id = id;
	pthread_mutex_unlock(&ready_queue_mutex);


	return 0;
}

int main(int argc, char *argv[]){
	int num_threads, n, chunk_size, i;

	/* Dealing with command line inputs */
	if(argc != 4){
		printf("Usage ./%s -p -n -c\n",argv[0]);
	 	return 1;	
	}
	else{
		num_threads = atoi(argv[1]);
		n=atoi(argv[2]);
		chunk_size=atoi(argv[3]);
	}
	

	/* Initialize the queue*/
	pthread_t slaves[num_threads];
	tail = malloc(sizeof(struct ready_queue));
	head = malloc(sizeof(struct ready_queue));
	tail->next = head;
	
	for(i=0;i<num_threads;i++){
		pthread_create(&slaves[i],NULL,slaveLogic,(void*)slaves[i]);
	}
	
	/*Initialize the numbers*/
	number_array = malloc(sizeof(struct numbers)*(n-1));
	for(i=0;i<0;i++){
		number_array[i]->number = i+1;
		number_array[i]->marked = 0;
	}







	/*Wait for all threads to complete*/
	for(i=0;i<num_threads;i++){
		pthread_join(slaves[i],NULL);
	}
	
	return 0;
}
