# **************************************************************************** #
# ***************************** VARIABLES: *********************************** #
# **************************************************************************** #
CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = so_long

SRC = src/so_long.c \
	src/error_utils.c src/free_utils.c \
	src/map_utils.c src/format_validation_utils.c src/path_validation_utils.c \
	src/movement_utils.c \
	src/game_init.c src/game_loop.c src/game_render.c \

OBJ = $(SRC:.c=.o)

LIBFT_REPO = external/libft
LIBFT_DIR = $(LIBFT_REPO)/libft
LIBFT = $(LIBFT_DIR)/libft.a

MLX_DIR = external/minilibx-linux
MLX = $(MLX_DIR)/libmlx_Linux.a

INCLUDE = -Iinclude -I$(LIBFT_DIR) -I$(MLX_DIR)

# **************************************************************************** #
# *****************************	RULES: *************************************** #
# **************************************************************************** #
all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) bonus

$(MLX):
	@if [ ! -f "$(MLX_DIR)/Makefile" ]; then \
		echo "Error: MiniLibX submodule is not initialized."; \
		echo "Run: git submodule update --init --recursive"; \
		exit 1; \
	fi
	$(MAKE) -C $(MLX_DIR)

$(OBJ): $(MLX)

$(NAME): $(OBJ) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX) -lXext -lX11 -lm -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean
	@if [ -d "$(MLX_DIR)" ]; then $(MAKE) -C $(MLX_DIR) clean; fi

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
