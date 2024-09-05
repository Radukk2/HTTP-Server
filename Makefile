CC=g++
CFLAGS=-I.

client: client.cpp requests.cpp helpers.c buffer.c
	$(CC) -o client client.cpp requests.cpp helpers.c buffer.c -Wall

run: client
	./client

clean:
	rm -f *.o client
