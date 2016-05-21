CC = gcc
CFLAGS =
LIBS = -lpthread

PROGS = server client
HEADERS = includes.h ownthread.h

.PHONY: all clean

all: ${PROGS}

server: server.o ${HEADERS}
	${CC} ${FLAGS} -o $@ server.o ${LIBS}

client: client.o ${HEADERS}
	${CC} ${FLAGS} -o $@ client.o ${LIBS}

clean:
	rm *.o ${PROGS}
