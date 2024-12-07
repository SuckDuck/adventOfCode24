CC=gcc
CFLAGS=
BIN_NAME=day3

all: ${BIN_NAME} 

%.o:%.c
	${CC} ${CFLAGS} -c -o $@ $<

${BIN_NAME}: main.o utils.o
	${CC} -o ${BIN_NAME} utils.o main.o

clean:
	rm *.o
	rm -f ${BIN_NAME}

clear: clean