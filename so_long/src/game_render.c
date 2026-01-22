#include "so_long.h"

static void	insert_image(t_game *game, int i, int j)
{
	int		img_i;
	int		img_j;
	char	cell;
	t_image	*img;

	img_i = i * IMG_SIZE;
	img_j = j * IMG_SIZE;
	cell = game->map.grid[j][i];
	if (cell == '1')
		img = &game->w;
	else if (cell == 'E')
		img = &game->e;
	else if (cell == 'C')
		img = &game->c;
	else
		img = &game->f;
	mlx_put_image_to_window(game->mlx, game->win, img->ptr, img_i, img_j);
}

void	render_game(t_game *game)
{
	int		i;
	int		j;
	int		img_p_i;
	int		img_p_j;
	t_image	*img_p;

	img_p_i = game->p_i * IMG_SIZE;
	img_p_j = game->p_j * IMG_SIZE;
	img_p = &game->p;
	j = 0;
	while (j < game->map.height)
	{
		i = 0;
		while (i < game->map.width)
		{
			insert_image(game, i, j);
			i++;
		}
		j++;
	}
	mlx_put_image_to_window(game->mlx, game->win, img_p->ptr, img_p_i, img_p_j);
}
