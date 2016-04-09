# Sieve_Of_Eratosthenes

The program starts with three parameters: n (the max number we want to find out about prime numbers), p (the number of slave threads), and c (chunk size). The program prints out whether n is a prime number.

The master and slave threads communicate via conditional variables. The program keeps a ready queue, which is a list of thread ids (ranging from 0 to p-1) that are ready for more work. The master takes one thread from the ready queue at a time, and send to it the next chunk to be worked on.
