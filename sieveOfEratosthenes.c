#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int main(int argc, char *argv[]){
	int num_threads;

	if(argc != 1){
		printf("Usage ./%s <number of threads>",argv[0]);
	 	return 1;	
	}
	else num_threads = argv[1];
}
