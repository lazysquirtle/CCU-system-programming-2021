SHELL = /bin/bash
CC = gcc 
CFLAGS = -g -pthread
SRC = $(wildcard *.c)
EXE = $(patsubst %.c, %, $(SRC))

all: ${EXE}	flock.db  lockf.db

%: 	%.c
	${CC} ${CFLAGS} $@.c -o $@

clean:
	rm ${EXE} 
	rm flock.db
	rm lockf.db

flock.db:
	echo "3500" > flock.db
lockf.db:
	echo "3500" > lockf.db
