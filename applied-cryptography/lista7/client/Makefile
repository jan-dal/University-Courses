CC 		= gcc
CFLAGS 	= -std=gnu17 -Wall -Wextra -c
LFLAGS  = -lssl -lcrypto -lreadline

all: client

client: obj
	${CC} ${LFLAGS} *.o -o client

obj: *.c *.h
	${CC} ${CFLAGS} *.c

clean:
	rm -rf *.o

distclean: clean
	rm -rf client
