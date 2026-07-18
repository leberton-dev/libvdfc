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

TEST_SRC = test/test_lexer.c test/test_read_file.c test/test_parser.c test/test_node_get.c test/test_fixtures.c
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

.PHONY: all clean fclean re test
