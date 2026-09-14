#include "so_long.h"

int	handle_close(t_game *game)
{
	mlx_destroy_image(game->mlx, game->w.ptr);
	mlx_destroy_image(game->mlx, game->e.ptr);
	mlx_destroy_image(game->mlx, game->p.ptr);
	mlx_destroy_image(game->mlx, game->c.ptr);
	mlx_destroy_image(game->mlx, game->f.ptr);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
	}
	free_map(&game->map);
	exit(0);
}

static void	move_player(t_game *game, int dir_i, int dir_j)
{
	int		nxt_i;
	int		nxt_j;
	char	cell;

	nxt_i = game->p_i + dir_i;
	nxt_j = game->p_j + dir_j;
	if (next_blocked(game, nxt_i, nxt_j))
		return ;
	cell = game->map.grid[nxt_j][nxt_i];
	if (cell == 'C')
		get_collectible(game, nxt_i, nxt_j);
	if (can_exit(game, cell))
		return ;
	game->p_i = nxt_i;
	game->p_j = nxt_j;
	game->mv_count++;
	ft_printf("Current move count: %d moves.\n", game->mv_count);
	render_game(game);
}

int	handle_key(int key, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (key == 65307)
		handle_close(game);
	else if (key == 'w' || key == 119 || key == 65362)
		move_player(game, 0, -1);
	else if (key == 'a' || key == 97 || key == 65361)
		move_player(game, -1, 0);
	else if (key == 's' || key == 115 || key == 65364)
		move_player(game, 0, 1);
	else if (key == 'd' || key == 100 || key == 65363)
		move_player(game, 1, 0);
	return (0);
}

void	run_game(t_game *game)
{
	render_game(game);
	mlx_key_hook(game->win, handle_key, game);
	mlx_hook(game->win, 17, 0, handle_close, game);
	mlx_loop(game->mlx);
}
