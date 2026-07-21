NAME = libvdfc.a
SRCS = src/vdf.c src/lexer.c src/parser.c src/node.c
OBJDIR = build
OBJS = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRCS))

CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDE = -I./include
AR = ar rcs
RM = rm -rf
HEADER = include/vdfc/vdf.h include/vdfc/errors.h include/vdfc/token.h include/vdfc/lexer.h include/vdfc/node.h include/vdfc/parser.h

TEST_SRC = test/test_lexer.c test/test_read_file.c test/test_parser.c test/test_fixtures.c \
		   test/node/test_get_bool.c \
		   test/node/test_get_bool_recursive.c \
		   test/node/test_get_int.c \
		   test/node/test_get_int_recursive.c \
		   test/node/test_get_string.c \
		   test/node/test_get_string_recursive.c \
		   test/node/test_get_long.c \
		   test/node/test_get_long_recursive.c \
		   test/node/test_get_long_long.c \
		   test/node/test_get_long_long_recursive.c \
		   test/node/test_get_ull.c \
		   test/node/test_get_ull_recursive.c
TEST_BIN = $(OBJDIR)/test_runner
CRITERION_FLAGS = $(shell pkg-config --cflags --libs criterion)

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

$(OBJDIR)/%.o: src/%.c $(HEADER) | $(OBJDIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) $(OBJDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

test: $(NAME) | $(OBJDIR)
	@$(CC) -fsanitize=address $(CRITERION_FLAGS) $(CFLAGS) $(INCLUDE) -I./test $(TEST_SRC) $(SRCS) -o $(TEST_BIN)
	@ASAN_OPTIONS=abort_on_error=1 ./$(TEST_BIN)

install: $(NAME)
	source scripts/install.sh && _install

uninstall:
	source scripts/install.sh && _pop_install



.PHONY: all clean fclean re test
