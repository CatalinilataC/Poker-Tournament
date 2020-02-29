CC=gcc
CFLAGS=-Wall -Wextra -c
EXE=poker_tournament
build: sursa.o
	$(CC) $^ -o $(EXE)
sursa.o: sursa.c
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -rf sursa.c $(EXE)
