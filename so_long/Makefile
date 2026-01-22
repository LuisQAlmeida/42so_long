#******************************************************#
#						VARIABLES                      #
#******************************************************#
CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = so_long

SRC = src/so_long.c src/error_utils.c src/free_utils.c src/map_utils.c \
	src/format_validation_utils.c src/path_validation_utils.c \
	src/game_init.c src/game_loop.c src/game_render.c \
	src/movement_utils.c

OBJ = $(SRC:.c=.o)

INCLUDE = -Iinclude -Ilibft -Iminilibx-linux

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

MLX_DIR = minilibx-linux
MLX = $(MLX_DIR)/libmlx_Linux.a

#******************************************************#
#                       RULES                          #
#******************************************************#
all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR) bonus

$(MLX):
	$(MAKE) -C $(MLX_DIR)

$(NAME): $(OBJ) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX) -lXext -lX11 -lm -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean
	$(MAKE) -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
