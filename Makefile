NAME = 		pipex
SRC = 		pipex.c \
			utils.c \
			parser.c \
			forks.c \
			path.c \
			here_doc.c \
			errors.c \
			debug.c
SRC_DIR =	src
OBJ_DIR	= 	$(SRC_DIR)/obj
INCL = 		-I$(SRC_DIR)/incl
C_FLAGS = 	-g -fsanitize=address -Wall -Wextra -Werror
OBJ = 		$(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

bonus: all

$(NAME): $(OBJ)
	$(MAKE) -C libft
	$(CC) $^ $(C_FLAGS) -Llibft -lft -o $@

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $< $(C_FLAGS) $(INCL) -Ilibft -c -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) fclean -C libft
	rm -f $(NAME)

re: clean all
