ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc.so

#path
SRC_DIR = ./srcs
INC_DIR = ./includes
OBJ_DIR = ./objs

# compiler
CC = gcc
CFLAGS = -Wall -Werror -Wextra -fPIC
INCLUDES = -I $(INC_DIR)

# srcs
SRCS = malloc.c\
	free.c\
	realloc.c\
	page.c\
	show_alloc_mem.c\
	init.c\
	utils.c\

SRC_BUDDY_BLOCK = allocate.c\
	block_meta.c\
	free.c\

SRC_MEMORY_POOL = allocate.c\

SRC_LIBC = ft_memset.c\
	ft_memcpy.c\
	ft_putchar.c\
	ft_putstr.c\

TEST = srcs/**/*.test.cpp srcs/*.test.cpp

# objs
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
OBJS += $(addprefix $(OBJ_DIR)/, $(addprefix BUDDY_BLOCK_, $(SRC_BUDDY_BLOCK:.c=.o)))
OBJS += $(addprefix $(OBJ_DIR)/, $(addprefix MEMORY_POOL_, $(SRC_MEMORY_POOL:.c=.o)))
OBJS += $(addprefix $(OBJ_DIR)/, $(addprefix LIBC_, $(SRC_LIBC:.c=.o)))

# compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(OBJ_DIR)/BUDDY_BLOCK_%.o: $(SRC_DIR)/buddy_block/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(OBJ_DIR)/MEMORY_POOL_%.o: $(SRC_DIR)/memory_pool/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(OBJ_DIR)/LIBC_%.o: $(SRC_DIR)/libc/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# command
all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -shared -o libft_malloc_$(HOSTTYPE).so $(OBJS)
	ln -sf libft_malloc_$(HOSTTYPE).so $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) libft_malloc_$(HOSTTYPE).so

re: fclean all

def = ''

test: $(TEST)
	g++\
		-Wall -Wextra -std=c++11\
		$(def)\
		-lgtest\
		$(INCLUDES)\
		$(SRC_DIR)/**/*.c $(SRC_DIR)/*.c $(TEST)\
		-o $@

.PHONY: test
