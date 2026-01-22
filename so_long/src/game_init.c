#include "so_long.h"

static void	init_game_params(t_game *game, t_map *map)
{
	game->mlx = NULL;
	game->win = NULL;
	game->w.ptr = NULL;
	game->f.ptr = NULL;
	game->e.ptr = NULL;
	game->c.ptr = NULL;
	game->p.ptr = NULL;
	game->p_i = 0;
	game->p_j = 0;
	game->c_total = 0;
	game->c_count = 0;
	game->mv_count = 0;
	game->map = *map;
	map->grid = NULL;
	map->width = 0;
	map->height = 0;
}

static void	init_game_state(t_game *game)
{
	int	i;
	int	j;

	j = 0;
	while (j < game->map.height)
	{
		i = 0;
		while (i < game->map.width)
		{
			if (game->map.grid[j][i] == 'P')
			{
				game->p_i = i;
				game->p_j = j;
			}
			if (game->map.grid[j][i] == 'C')
				game->c_total++;
			i++;
		}
		j++;
	}
}

static void	init_game_window(t_game *game)
{
	int	win_w;
	int	win_h;

	game->mlx = mlx_init();
	if (!game->mlx)
	{
		free_map(&game->map);
		error_exit(ERR_MLX);
	}
	win_w = game->map.width * IMG_SIZE;
	win_h = game->map.height * IMG_SIZE;
	game->win = mlx_new_window(game->mlx, win_w, win_h, "so_long");
	if (!game->win)
	{
		free_map(&game->map);
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		error_exit(ERR_MLX);
	}
}

static void	load_image(t_game *game, t_image *img, char *path)
{
	int	img_w;
	int	img_h;

	img_w = IMG_SIZE;
	img_h = IMG_SIZE;
	img->ptr = mlx_xpm_file_to_image(game->mlx, path, &img_w, &img_h);
	if (!img->ptr)
	{
		destroy_images(game);
		free_map(&game->map);
		if (game->win)
			mlx_destroy_window(game->mlx, game->win);
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		error_exit(ERR_MLX);
	}
	img->width = img_w;
	img->height = img_h;
}

void	init_game(t_game *game, t_map *map)
{
	init_game_params(game, map);
	init_game_state(game);
	init_game_window(game);
	load_image(game, &game->f, "textures/floor.xpm");
	load_image(game, &game->w, "textures/wall.xpm");
	load_image(game, &game->e, "textures/exit.xpm");
	load_image(game, &game->c, "textures/collectible.xpm");
	load_image(game, &game->p, "textures/player.xpm");
}
