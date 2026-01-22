#include "so_long.h"

static int	valid_shape(t_map *map)
{
	int	j;

	j = 0;
	while (j < map->height)
	{
		if ((int)ft_strlen(map->grid[j]) != map->width)
			return (0);
		j++;
	}
	return (1);
}

static int	valid_walls(t_map *map)
{
	int	i;
	int	j;
	int	i_max;
	int	j_max;

	i = 0;
	j = 0;
	i_max = map->width - 1;
	j_max = map->height - 1;
	while (i < map->width)
	{
		if (map->grid[0][i] != '1' || map->grid[j_max][i] != '1')
			return (0);
		i++;
	}
	while (j < map->height)
	{
		if (map->grid[j][0] != '1' || map->grid[j][i_max] != '1')
			return (0);
		j++;
	}
	return (1);
}

static int	valid_count_chars(char a, int *p, int *e, int *c)
{
	if (a == 'P')
	{
		(*p)++;
		return (1);
	}
	else if (a == 'E')
	{
		(*e)++;
		return (1);
	}
	else if (a == 'C')
	{
		(*c)++;
		return (1);
	}
	else if (a == '0' || a == '1')
		return (1);
	else
		return (0);
}

static int	valid_chars_counts(t_map *map)
{
	int	i;
	int	j;
	int	p;
	int	e;
	int	c;

	p = 0;
	e = 0;
	c = 0;
	j = 0;
	while (j < map->height)
	{
		i = 0;
		while (i < map->width)
		{
			if (valid_count_chars(map->grid[j][i], &p, &e, &c) == 0)
				return (0);
			i++;
		}
		j++;
	}
	if (p != 1 || e != 1 || c < 1)
		return (0);
	return (1);
}

void	validate_map_format(t_map *map)
{
	if (!map || !map->grid)
		error_exit(ERR_MAP_FORMAT);
	if (map->height < 3 || map->width < 3)
	{
		free_map(map);
		error_exit(ERR_MAP_FORMAT);
	}
	if (!valid_shape(map) || !valid_walls(map))
	{
		free_map(map);
		error_exit(ERR_MAP_FORMAT);
	}
	if (!valid_chars_counts(map))
	{
		free_map(map);
		error_exit(ERR_MAP_FORMAT);
	}
}
