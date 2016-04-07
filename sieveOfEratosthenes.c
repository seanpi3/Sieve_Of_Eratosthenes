#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_cond_t cond;
pthread_mutex_t mutex;
pthread_mutex_t ready_queue_mutex;
pthread_barrier_t init_barrier;
int* marked;

typedef struct ready_queue{
	pthread_t id;
	struct ready_queue *next;
};

struct ready_queue *head;
struct ready_queue *tail;

typedef struct work{
	int lower_bound;
	int upper_bound;
	int k;
};

struct work **jobs;

typedef struct slave{
	int slave_num;
	pthread_t id;
};

struct slave **slaves;

/*Logic for the slave threads*/
void *slaveLogic(void *arg){
	int ready;
	struct slave *slave_info = (struct slave*)arg;

	printf("Thread: %d waiting for all threads to be ready\n",slave_info->slave_num);
	pthread_barrier_wait(&init_barrier);

	/*Add self to the ready queue*/
	pthread_mutex_lock(&ready_queue_mutex);
	tail->next = malloc(sizeof(struct ready_queue));
	tail = tail->next;
	tail->id = slave_info->id;
	pthread_mutex_unlock(&ready_queue_mutex);

	printf("Yo im ready dawg\n");
	return 0;
}


void assign_work(int k, int chunk_size){
	
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
	
	pthread_barrier_init(&init_barrier,NULL,num_threads);

	/* Initialize the slaves*/
	slaves = malloc(num_threads);
	for(i=0;i<num_threads;i++){
					slaves[i] = malloc(sizeof(struct slave));
	}
	tail = malloc(sizeof(struct ready_queue));
	head = malloc(sizeof(struct ready_queue));
	tail->next = head;
	
	for(i=0;i<num_threads;i++){
		if(i==4) printf("seg fault inc\n");
		pthread_create(&slaves[i]->id,NULL,slaveLogic,(void*)slaves[i]);
		slaves[i]->slave_num = i;
	}

	printf("I got here \n");

	
	/*Initialize the numbers*/
	marked = malloc(sizeof(int)*(n+1));
	for(i=0;i<n+1;i++)
		marked[i] = 0;
	
	/*Initialize work*/
	jobs = malloc(num_threads);
	for(i=0;i<num_threads;i++)
		jobs[i] = malloc(sizeof(struct work));





	/*Wait for all threads to complete*/
	for(i=0;i<num_threads;i++){
		pthread_join(slaves[i],NULL);
	}
	
	return 0;
}
