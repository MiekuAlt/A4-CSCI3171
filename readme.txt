************************************************
Assignment 4 - CSCI3171

Chaoran Zhou    B00551572
Michael Altair  B00599791

************************************************
How to compile and start the server:

gcc server.c -o server.out

./server.out 8080
(Or any socket number you choose)

************************************************
How to compile and start the client:

gcc client.c -o client.out

on a local host:
./client.out localhost 8080
(The same socket number you chose for the server)

on a remote host:
./client.out (the IP address of the server side) 8080
(The same socket number you chose for the server)

************************************************
