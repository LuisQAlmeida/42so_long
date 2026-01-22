#include "so_long.h"

static char	**get_grid_copy(t_map *map)
{
	int		j;
	char	**copy;

	copy = (char **)malloc(sizeof(char *) * map->height);
	if (!copy)
		error_exit(ERR_MALLOC);
	j = 0;
	while (j < map->height)
	{
		copy[j] = ft_strdup(map->grid[j]);
		if (!copy[j])
		{
			free_grid(copy, j);
			error_exit(ERR_MALLOC);
		}
		j++;
	}
	return (copy);
}

static void	get_p_c(t_map *map, int *p_i, int *p_j, int *c_total)
{
	int	i;
	int	j;

	*p_i = -1;
	*p_j = -1;
	*c_total = 0;
	j = 0;
	while (j < map->height)
	{
		i = 0;
		while (i < map->width)
		{
			if (map->grid[j][i] == 'P')
			{
				*p_i = i;
				*p_j = j;
			}
			else if (map->grid[j][i] == 'C')
				(*c_total)++;
			i++;
		}
		j++;
	}
	if (*p_i == -1 || *p_j == -1)
		error_exit(ERR_MAP_FORMAT);
}

static void	flood_fill(t_path *path, int i, int j)
{
	char	cell;

	if (i < 0 || j < 0 || i >= path->map->width || j >= path->map->height)
		return ;
	cell = path->grid[j][i];
	if (cell == '1' || cell == 'V')
		return ;
	if (cell == 'C')
		path->found_c++;
	if (cell == 'E')
		path->found_e = 1;
	path->grid[j][i] = 'V';
	flood_fill(path, i + 1, j);
	flood_fill(path, i - 1, j);
	flood_fill(path, i, j + 1);
	flood_fill(path, i, j - 1);
}

void	validate_map_path(t_map *map)
{
	int		p_i;
	int		p_j;
	int		c_total;
	t_path	path;

	if (!map || !map->grid)
		error_exit(ERR_MAP_PATH);
	get_p_c(map, &p_i, &p_j, &c_total);
	path.map = map;
	path.grid = get_grid_copy(map);
	path.found_c = 0;
	path.found_e = 0;
	flood_fill(&path, p_i, p_j);
	free_grid(path.grid, map->height);
	if (path.found_c != c_total || !path.found_e)
	{
		free_map(map);
		error_exit(ERR_MAP_PATH);
	}
}
