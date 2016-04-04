#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *slaveLogic(void *arg){
	return 0;
}

int main(int argc, char *argv[]){
	int num_threads;
	int n;

	if(argc != 4){
		printf("Usage ./%s -p -n -c\n",argv[0]);
	 	return 1;	
	}
	else{
		num_threads = atoi(argv[1]);
		n=atoi(argv[2]);
	}
	
	pthread_t slaves[num_threads];
	int i;
	for(i=0;i<num_threads;i++){
		pthread_create(&slaves[i],NULL,slaveLogic,NULL);
	}
		





	for(i=0;i<num_threads;i++){
		pthread_join(slaves[i],NULL);
	}
	
	return 0;
}
