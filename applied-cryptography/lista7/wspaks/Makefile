CC 		= gcc
CFLAGS 	= -std=gnu17 -Wall -Wextra -c
LFLAGS  = -lssl -lcrypto

all: server

server: obj
	${CC} ${LFLAGS} *.o -o server

obj: *.c *.h
	${CC} ${CFLAGS} *.c

clean:
	rm -rf *.o

distclean: clean
	rm -rf server
