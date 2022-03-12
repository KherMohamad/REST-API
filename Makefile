CC = gcc
CFLAGS = -I.

build: client.c requests.c helpers.c buffer.c parson.c 
	$(CC) -o client client.c requests.c helpers.c buffer.c parson.c

run: client
	./client

clean:
	rm -f *.o client
