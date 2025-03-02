NAME = philo

SRCS = main.c philosophers.c utils.c philo_utils.c \
		extra_utils.c checkers.c

HEADERS = philosophers.h

OBJS = $(SRCS:.c=.o)

CC = cc

CFLAGS = -Wall -Wextra -Werror -g

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.SECONDARY: $(OBJS)

.PHONY: all clean fclean re