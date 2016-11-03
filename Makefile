ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CC=clang
FLAGS=-Wall -Wextra -Werror -Wconversion

NAME=libft_malloc_$(HOSTTYPE).so
TEST=test

LIBS_DIR=./libs
DIR_LIBFT=$(LIBS_DIR)/libft
DIR_PRINTF=$(LIBS_DIR)/printf
DIR_LIST=$(LIBS_DIR)/list

LIBS=-L $(DIR_PRINTF) -lprintf -L $(DIR_LIBFT) -lft -L $(DIR_LIST) -llist

SRC_DIR=srcs
INCLUDES=-I ./ -I ./includes -I $(DIR_LIBFT) -I $(DIR_LIST) -I $(DIR_PRINTF)

BUILD_DIR= __build

SRC=log.c				\
		context.c		\
		region.c		\
		quantum.c		\
		malloc.c		\
		free.c			\
		show_mem.c	\


OBJ=$(addprefix $(BUILD_DIR)/,$(SRC:.c=.o))

all:$(BUILD_DIR) $(NAME)

$(BUILD_DIR):
	@mkdir -p $@

exec:
	@make -C $(DIR_LIBFT)
	@make -C $(DIR_PRINTF)
	@make -C $(DIR_LIST)

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.c
	@$(CC) $(FLAGS) -fPIC -c $< -o $@ $(INCLUDES)

$(NAME):exec $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) $(LIBS) -shared -o $@
	@echo "$@ was created"
	@ln -f -s libft_malloc_$(HOSTTYPE).so libft_malloc.so

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f libft_malloc.so

fclean: clean
	@rm -f $(NAME) $(TEST)
	@make $@ -C $(DIR_LIBFT)
	@make $@ -C $(DIR_PRINTF)
	@make $@ -C $(DIR_LIST)

re: fclean all

test: $(NAME)
	$(CC) test.c $(OBJ) $(LIBS) -o $(TEST)
