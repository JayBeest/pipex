NAME = 		pipex
NO_BONUS =	pipex.c
DO_BONUS =	pipex_bonus.c
SHARED =	utils.c \
			parser.c \
			forks.c \
			path.c \
			here_doc.c \
			errors.c \
			dupstd.c

ifdef BONUS
SRC = $(DO_BONUS) $(SHARED)
else
SRC = $(NO_BONUS) $(SHARED)
endif

SRC_DIR =	src
OBJ_DIR	= 	$(SRC_DIR)/obj
INCL = 		-I$(SRC_DIR)/incl
C_FLAGS = 	-Wall -Wextra -Werror #-g -fsanitize=address

OBJ = 		$(SRC:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

bonus:
	$(MAKE) BONUS=1 all
	touch src/pipex.c

$(NAME): $(OBJ)
	$(MAKE) -C libft
	$(CC) $^ $(C_FLAGS) -Llibft -lft -o $@
	touch src/pipex_bonus.c

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $< $(C_FLAGS) $(INCL) -Ilibft -c -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) fclean -C libft
	rm -f $(NAME)

re: clean all
