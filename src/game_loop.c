#include "so_long.h"

#define KEY_ESC 65307
#define KEY_LEFT 65361
#define KEY_UP 65362
#define KEY_RIGHT 65363
#define KEY_DOWN 65364
#define EVENT_DESTROY 17

int	handle_close(t_game *game)
{
	destroy_images(game);
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
	{
		game->mv_count++;
		ft_printf("Current move count: %d moves.\n", game->mv_count);
		handle_close(game);
		return ;
	}
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
	if (key == KEY_ESC)
		handle_close(game);
	else if (key == 'w' || key == KEY_UP)
		move_player(game, 0, -1);
	else if (key == 'a' || key == KEY_LEFT)
		move_player(game, -1, 0);
	else if (key == 's' || key == KEY_DOWN)
		move_player(game, 0, 1);
	else if (key == 'd' || key == KEY_RIGHT)
		move_player(game, 1, 0);
	return (0);
}

void	run_game(t_game *game)
{
	render_game(game);
	mlx_key_hook(game->win, handle_key, game);
	mlx_hook(game->win, EVENT_DESTROY, 0, handle_close, game);
	mlx_loop(game->mlx);
}
