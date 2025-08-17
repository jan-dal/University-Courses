CC 		= gcc
CFLAGS 	= -std=gnu17 -Wall -Wextra -c

all: server

server: obj
	${CC} *.o -o server

obj: main.c response.c response.h request.c request.h
	${CC} ${CFLAGS} main.c response.c request.c

clean:
	rm -rf *.o

distclean: clean
	rm -rf server
