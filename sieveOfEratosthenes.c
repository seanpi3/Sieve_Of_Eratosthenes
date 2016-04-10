#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>

pthread_mutex_t ready_queue_mutex;
pthread_barrier_t init_barrier;
int* marked;

typedef struct ready_queue{
	int id;
	struct ready_queue *next;
};

struct ready_queue *head;
struct ready_queue *tail;
struct ready_queue *empty_queue;

typedef struct work{
	int lower_bound;
	int upper_bound;
	int k;
	int work_ready;
};

struct work **jobs;

typedef struct slave{
	int slave_num;
	pthread_t id;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
};

struct slave **slaves;

/*Logic for the slave threads*/
void *slaveLogic(void *arg){
	struct slave *slave_info = (struct slave*)arg;
	struct ready_queue *queue_position = malloc(sizeof(struct ready_queue));
	struct work *slave_work = jobs[slave_info->slave_num];
	int i;
	queue_position->id = slave_info->slave_num;

	pthread_barrier_wait(&init_barrier);

	/*Main Work Loop*/
	for(;;){
		/*Add self to the ready queue*/
		pthread_mutex_lock(&ready_queue_mutex);
		if(tail == empty_queue){
			head = queue_position;
			tail = head;
		}
		else{
			tail->next = queue_position;
			tail = tail->next;
			tail->next = empty_queue;
		}	
		pthread_mutex_unlock(&ready_queue_mutex);

		/*Check for work*/
		pthread_mutex_lock(&slave_info->mutex);
		while(slave_work->work_ready != 1)
			pthread_cond_wait(&slave_info->cond,&slave_info->mutex);
		pthread_mutex_unlock(&slave_info->mutex);
	
		/*Do work*/
		for(i=slave_work->lower_bound;i<=slave_work->upper_bound;i++){
			if(i%(slave_work->k) == 0){
				marked[i] = 1;
			}
		}
		slave_work->work_ready = 0;
	}	


	return 0;
}


int main(int argc, char *argv[]){
	int num_threads, n, chunk_size, i;
	int chunks,j;

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

	struct timeval start_time, end_time;
	gettimeofday(&start_time,NULL);
	printf("Calculating whether %d is prime using %d threads with a chunk size of %d.\n",n,num_threads,chunk_size);
	
	pthread_barrier_init(&init_barrier,NULL,num_threads+1);

	/*Initialize the work*/
	jobs = malloc(sizeof(struct work*)*num_threads);
	for(i=0;i<num_threads;i++){
		jobs[i] = malloc(sizeof(struct work));
		jobs[i]->work_ready = 0;
	}

	/*Initialize the numbers*/
	marked = malloc(sizeof(int)*(n+1));
	for(i=0;i<n+1;i++)
		marked[i] = 0;

	/* Initialize the slaves*/
	empty_queue = malloc(1);
	head = empty_queue;
	tail = empty_queue;

	slaves = malloc(sizeof(struct slave*)*num_threads);
	for(i=0;i<num_threads;i++){
		slaves[i] = malloc(sizeof(struct slave));
	}
	
	for(i=0;i<num_threads;i++){
		slaves[i]->slave_num = i;
		pthread_cond_init(&slaves[i]->cond,NULL);
		pthread_create(&slaves[i]->id,NULL,slaveLogic,(void*)slaves[i]);
	}

	pthread_barrier_wait(&init_barrier);


	/*Assign Work*/
	int curr_id;
	double percent;
	long int elapsed;
	for(i=2;i<(int)(floor(sqrt(n)));i++){
		if(!marked[i]){
			chunks = ceil((n-(i*i))/chunk_size);
			for(j=0;j<=chunks;j++){
				pthread_mutex_lock(&ready_queue_mutex);
				while(head==empty_queue){
					pthread_mutex_unlock(&ready_queue_mutex);
					pthread_mutex_lock(&ready_queue_mutex);
				}
				curr_id = head->id;
				if(head == tail){
					head=empty_queue;
					tail=empty_queue;
				}
				else head=head->next;
				pthread_mutex_unlock(&ready_queue_mutex);
				jobs[curr_id]->k=i;
				jobs[curr_id]->lower_bound = (i*i)+(chunk_size*j);
				jobs[curr_id]->upper_bound = ((i*i)+(chunk_size*(j+1)))-1;
				if(jobs[curr_id]->upper_bound > n) jobs[curr_id]->upper_bound = n;
				jobs[curr_id]->work_ready = 1;
				pthread_cond_broadcast(&slaves[curr_id]->cond);
			}
		}

		percent = (i/sqrt(n))*100;
		
		gettimeofday(&end_time,NULL);
		long int elapsed = (end_time.tv_usec + 1000000 *end_time.tv_sec) - (start_time.tv_usec + 1000000 * start_time.tv_sec); 
		printf("\r");
		printf("%02f%% completed. Time elapsed: %02ld.%06ld",percent,elapsed/1000000,elapsed%1000000);
	}
	printf("\n");
	for(i=0;i<num_threads;i++){
		if(jobs[i]->work_ready==1) i=0;
	}	

	gettimeofday(&end_time,NULL);

	elapsed = (end_time.tv_usec + 1000000 *end_time.tv_sec) - (start_time.tv_usec + 1000000 * start_time.tv_sec); 
	printf("Caclulation complete.\nTotal time elapsed:%ld seconds.\n",elapsed/1000000);
	/*for(i=3;i<=n;i+=2){
		if(marked[i]==0) printf("%d is prime\n",i);
	}*/
	if(marked[n]==0) printf("%d is prime\n",n);
	else printf("%d is not prime\n",n);

	return 0;
}
