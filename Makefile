CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2



mainClient: src/mainClient.c src/mainClient.h
	$(CC) $(CFLAGS) -o $@ $^

mainServer: src/mainServer.c src/mainServer.h
	$(CC) $(CFLAGS) -o $@ $^