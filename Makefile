CC = cc
CFLAGS = -Wall -Werror -Wextra -fsanitize=thread
SRC_FILES = *.c
OBJ_FILES = *.o


run:
	cc *.c -pthread
	./a.out 3 5000 5000 5000 5000 5000 5000 fifo
