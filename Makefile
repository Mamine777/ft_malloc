NAME    = libft_malloc.so
EXEC    = malloc

CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -fPIC -g
LDFLAGS = -shared -lpthread
EXEC_FLAGS = -lpthread

SRC = \
	src/ft_malloc.c \
	src/ft_free.c \
	src/realloc.c \
	src/malloc_utils/alloc.c \
	src/malloc_utils/utils.c \
	src/malloc_utils/zones.c

OBJDIR = obj
OBJ = $(SRC:src/%.c=$(OBJDIR)/%.o)
INC = -I./include

# ---------------- RULES ---------------- #

all: $(NAME) $(EXEC)

# Create obj folder if not exists
$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Build shared library
$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

# Build executable
$(EXEC): main.o $(NAME)
	$(CC) main.o -L. -lft_malloc -lpthread -o $(EXEC)

# Compile main.o
main.o: main.c
	$(CC) $(CFLAGS) $(INC) -c main.c -o main.o

clean:
	rm -f $(OBJ) main.o

fclean: clean
	rm -f $(NAME) $(EXEC)

re: fclean all

.PHONY: all clean fclean re