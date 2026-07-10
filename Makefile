NAME = libvdfc.a
SRCS = src/vdf.c src/lexer.c
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDE = -I./include
AR = ar rcs
RM = rm -f
HEADER = include/vdfc/vdf.h include/vdfc/errors.h include/vdfc/token.h include/vdfc/lexer.h

TEST_SRC = test/test_main.c test/test_read_file.c test/test_lexer.c test/test_utils.c
TEST_BIN = test/test_runner

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

test: $(NAME)
	$(CC) $(CFLAGS) $(INCLUDE) -I./test $(TEST_SRC) $(NAME) -o $(TEST_BIN)
	./$(TEST_BIN)

.PHONY: all clean fclean re test
