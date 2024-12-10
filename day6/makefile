CC=gcc
CFLAGS=-g
all: first second

board.o: board.c
	${CC} ${CFLAGS} -c -o board.o board.c

first: board.o first.c
	${CC} ${CFLAGS} -o first board.o first.c

second: board.o second.c
	${CC} ${CFLAGS} -o second board.o second.c

clean:
	rm -f first second *.o

clear: clean