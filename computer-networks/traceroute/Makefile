CC 		= gcc
CFLAGS 	= -std=gnu17 -Wall -Wextra -c

all: traceroute

traceroute: obj
	${CC} *.o -o traceroute

obj: packets.c main.c packets.h
	${CC} ${CFLAGS} packets.c main.c

clean:
	rm -rf *.o

distclean: clean
	rm -rf traceroute
