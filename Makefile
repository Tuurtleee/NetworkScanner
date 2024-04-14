CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2 -std=c99 -g -Wshadow -Wstrict-prototypes



mainClient: src/mainClient.c src/mainClient.h
	$(CC) $(CFLAGS) -o $@ $^

mainServer: src/mainServer.c src/mainServer.h src/verticalScan.c src/verticalScan.h
	$(CC) $(CFLAGS) -o $@ $^