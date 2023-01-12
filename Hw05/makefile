SHELL = /bin/bash
CC = gcc
CFLAGS = -g
LIB = -lacl
SRC = $(wildcard *.c)
EXE = $(patsubst %.c, %, $(SRC))

all: ${EXE}

%:	%.c
	${CC} ${CFLAGS} $@.c ${LIB} -o $@

clean:
	rm ${EXE}

command:
	sudo chown root preBirthday
	sudo chmod +s ./preBirthday
