
FLAGS  = -Wall -g
CC     = gcc
PROG   = DES
OBJS   = main.o DES.o
all:	${PROG}

clean:
	rm ${PROG}
  
${PROG}:	${OBJS}
	${CC} ${FLAGS} ${OBJS} -o $@ && rm ${OBJS}

.c.o:
	${CC} ${FLAGS} $< -c

##########################

DES.o: DES.h DES.c

main.o: main.c

DES: main.o DES.o
