CC=gcc
CFLAGS=
OUTPUT0=first_half
OUTPUT1=second_half

all: ${OUTPUT0} ${OUTPUT1}

%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $<

${OUTPUT0}: utils.o ${OUTPUT0}.o
	${CC} -o ${OUTPUT0} utils.o ${OUTPUT0}.o

${OUTPUT1}: utils.o ${OUTPUT1}.o
	${CC} -o ${OUTPUT1} utils.o ${OUTPUT1}.o

clean:
	rm -f *.o
	rm -f first_half second_half

clear: clean