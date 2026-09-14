#include "so_long.h"

int	next_blocked(t_game *game, int nxt_i, int nxt_j)
{
	if (nxt_i < 0 || nxt_j < 0)
		return (1);
	if (nxt_i >= game->map.width || nxt_j >= game->map.height)
		return (1);
	if (game->map.grid[nxt_j][nxt_i] == '1')
		return (1);
	return (0);
}

void	get_collectible(t_game *game, int nxt_i, int nxt_j)
{
	game->c_count++;
	game->map.grid[nxt_j][nxt_i] = '0';
}

int	can_exit(t_game *game, char cell)
{
	if (cell != 'E')
		return (0);
	if (game->c_count != game->c_total)
		return (0);
	return (1);
}
