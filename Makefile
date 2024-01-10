CC = gcc
CFLAGS = -Wall -g
EXE = main

$(EXE): *.c
	$(CC) $(CFLAGS) -o $(EXE) main.c

clean:
	rm $(EXE)
