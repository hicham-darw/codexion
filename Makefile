CC = cc
CFLAGS = -Wall -Werror -Wextra
SRC_FILES = *.c
OBJ_FILES = *.o


run:
	cc *.c -pthread
	./a.out 5 5000 5000 5000 5000 5000 5000 fifo
