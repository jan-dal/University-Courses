CC 		= gcc
CFLAGS 	= -std=gnu17 -Wall -Wextra -c

all: server

server: obj
	${CC} *.o -o server

obj: *.c *.h
	${CC} ${CFLAGS} *.c

clean:
	rm -rf *.o

distclean: clean
	rm -rf server
