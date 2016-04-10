sieve: sieveOfEratosthenes.c
	gcc -pthread -Wall $< -o $@ -lm
clean:
	rm -f sieve *.o *~ core
