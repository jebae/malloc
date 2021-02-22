#path
SRC_DIR = ./srcs
INC_DIR = ./includes
# LIB_DIR = ./libs
# LIBFT = $(LIB_DIR)/libft

# compiler option
CFLAGS = -Wall -Werror -Wextra
INCLUDES = -I $(INC_DIR)
# LIBS = -L $(LIBFT) -lft

# srcs
TEST = srcs/**/*.test.cpp srcs/*.test.cpp

def = ''

test: $(TEST)
	g++\
		-Wall -Wextra -std=c++11\
		$(def)\
		-lgtest\
		$(INCLUDES)\
		$(SRC_DIR)/**/*.c $(TEST)\
		-o $@

.PHONY: test
