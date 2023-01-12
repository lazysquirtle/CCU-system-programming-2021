SHELL = /bin/bash
CC = gcc
CFLAGS = -g
LIB = -lacl
SRC = $(wildcard *.c)
EXE = $(patsubst %.c, %, $(SRC))

all: ${EXE}

%:	%.c
	${CC} ${CFLAGS} $@.c ${LIB} -o $@
	ln -s $@ ls
	ln -s $@ cp
	ln -s $@ cat
	ln -s $@ chown
	
clean:
	rm ${EXE}
	rm ls 
	rm cp 
	rm cat 
	rm chown 
