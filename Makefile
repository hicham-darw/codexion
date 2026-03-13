CC = cc
CFLAGS = -Wall -Werror -Wextra
SRC_FILES = *.c
OBJ_FILES = *.o


run:
	cc *.c -pthread
