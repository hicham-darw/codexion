CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -pthread 
NAME = codexion
SRC =  coder.c codexion.c compile.c create_threads.c destroy.c dongle.c \
	free.c heap.c heapify.c helper_functions_one.c helper_functions_two.c \
	initial_coders.c initial_dongles.c initial_heap.c initial_manager.c \
	initial_monitor.c initializer.c join_threads.c manager.c monitor.c \
	parser.c times.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

%.o:%.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@ -I.

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all run clean fclean re
