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

INCLUDE = -Iinclude -I$(LIBFT_DIR) -Iminilibx-linux

MLX_DIR = minilibx-linux
MLX = $(MLX_DIR)/libmlx_Linux.a
MLX_TGZ = minilibx-linux.tgz
MLX_REPO = https://github.com/42Paris/minilibx-linux.git
MLX_GIT_JUNK = $(MLX_DIR)/.git $(MLX_DIR)/.github $(MLX_DIR)/.gitignore

# **************************************************************************** #
# *****************************	RULES: *************************************** #
# **************************************************************************** #
all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) bonus

$(MLX):
	@if [ -d "$(MLX_DIR)" ]; then \
		echo "MiniLibX: found $(MLX_DIR)/"; \
	elif [ -f "$(MLX_TGZ)" ]; then \
		echo "MiniLibX: extracting and deleting $(MLX_TGZ)"; \
		tar -xzf "$(MLX_TGZ)"; \
		rm -f "$(MLX_TGZ)"; \
		echo "$(MLX_DIR) extracted successfully."; \
	elif command -v git >/dev/null 2>&1; then \
		echo "MiniLibX: cloning $(MLX_REPO)"; \
		git clone "$(MLX_REPO)" "$(MLX_DIR)"; \
		echo "$(MLX_DIR) clone created successfully."; \
	else \
		echo "Error: MiniLibX missing and git repo not available."; \
		exit 1; \
	fi
	@if [ -d "$(MLX_DIR)/.git" ]; then rm -rf "$(MLX_DIR)/.git"; fi
	@if [ -d "$(MLX_DIR)/.github" ]; then rm -rf "$(MLX_DIR)/.github"; fi
	@if [ -f "$(MLX_DIR)/.gitignore" ]; then rm -f "$(MLX_DIR)/.gitignore"; fi
	@$(MAKE) -C $(MLX_DIR)

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
