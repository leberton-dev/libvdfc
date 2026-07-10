NAME = libvdfc.a
SRCS = src/vdf.c src/lexer.c
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDE = -I./include
AR = ar rcs
RM = rm -f
HEADER = include/vdfc/vdf.h include/vdfc/errors.h include/vdfc/token.h include/vdfc/lexer.h

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
