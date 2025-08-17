CC 		= gcc
CFLAGS 	= -std=gnu17 -Wall -Wextra -c

all: router

router: obj
	${CC} *.o -o router

obj: main.c configuration.c configuration.h udp_datagram.c udp_datagram.h
	${CC} ${CFLAGS} main.c configuration.c udp_datagram.c

clean:
	rm -rf *.o

distclean: clean
	rm -rf router
