sieve: sieveOfEratosthenes.c
	gcc -pthread -Wall $< -o $@
clean:
	rm -f sieve *.o *~ core
