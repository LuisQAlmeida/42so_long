#include "so_long.h"

void	free_grid(char **row, int count)
{
	int	j;

	j = 0;
	if (!row)
		return ;
	while (j < count)
	{
		free(row[j]);
		j++;
	}
	free(row);
}

void	free_rows(t_list *row, int content)
{
	t_list	*tmp;

	while (row)
	{
		tmp = row->next;
		if (row->content && content)
			free(row->content);
		free(row);
		row = tmp;
	}
}

void	free_map(t_map *map)
{
	int	j;

	j = 0;
	if (!map || !map->grid)
		return ;
	while (j < map->height)
	{
		free(map->grid[j]);
		j++;
	}
	free(map->grid);
	map->grid = NULL;
	map->height = 0;
	map->width = 0;
}

void	destroy_images(t_game *game)
{
	if (!game->mlx)
		return ;
	if (game->f.ptr)
		mlx_destroy_image(game->mlx, game->f.ptr);
	if (game->w.ptr)
		mlx_destroy_image(game->mlx, game->w.ptr);
	if (game->e.ptr)
		mlx_destroy_image(game->mlx, game->e.ptr);
	if (game->c.ptr)
		mlx_destroy_image(game->mlx, game->c.ptr);
	if (game->p.ptr)
		mlx_destroy_image(game->mlx, game->p.ptr);
}
