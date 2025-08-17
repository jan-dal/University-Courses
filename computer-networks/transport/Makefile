CC 		= gcc
CFLAGS 	= -std=gnu17 -Wall -Wextra -c

all: transport

transport: obj
	${CC} *.o -o transport

obj: main.c udp_datagram.c udp_datagram.h utility.c utility.h
	${CC} ${CFLAGS} udp_datagram.c main.c utility.c

clean:
	rm -rf *.o

distclean: clean
	rm -rf transport
