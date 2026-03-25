CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -pthread
NAME = codexion
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(NAME)

run: $(NAME)
	./$(NAME) 3 5000 5000 5000 5000 5000 5000 fifo

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) codexion

re: fclean all

.PHONY: all run clean fclean re
