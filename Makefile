NAME        = libft_malloc.so
EXEC        = malloc

CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -fPIC
LDFLAGS     = -shared -lpthread
EXEC_FLAGS  = -lpthread

# ---------------- SOURCES ---------------- #

SRC = \
	src/ft_malloc.c \
	src/ft_free.c \
	src/realloc.c \
	src/malloc_utils/alloc.c \
	src/malloc_utils/utils.c \
	src/malloc_utils/zones.c

OBJ = $(SRC:.c=.o)

INC = -I./include

# ---------------- RULES ---------------- #

all: $(NAME)

# Shared library
$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

# Executable
$(EXEC): $(OBJ)
	$(CC) $(OBJ) $(EXEC_FLAGS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) $(EXEC)

re: fclean all

.PHONY: all clean fclean re
