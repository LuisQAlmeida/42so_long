#include "so_long.h"

static void	remove_newline(char *content)
{
	size_t	length;

	if (!content)
		return ;
	length = ft_strlen(content);
	if (length > 0 && content[length - 1] == '\n')
		content[length - 1] = '\0';
}

static t_list	*get_rows(int fd)
{
	char	*content;
	t_list	*rows;
	t_list	*row;

	content = get_next_line(fd);
	rows = NULL;
	while (content)
	{
		remove_newline(content);
		row = ft_lstnew(content);
		if (!row)
		{
			free(content);
			free_rows(rows, 1);
			error_exit(ERR_MALLOC);
		}
		ft_lstadd_back(&rows, row);
		content = get_next_line(fd);
	}
	return (rows);
}

static char	**get_grid(t_list *rows, t_map *map)
{
	int		j;
	char	**grid;
	t_list	*row;

	j = 0;
	map->height = ft_lstsize(rows);
	row = rows;
	if (map->height == 0)
		return (NULL);
	grid = (char **)malloc(sizeof(char *) * map->height);
	if (!grid)
		return (NULL);
	while (row)
	{
		grid[j] = ft_strdup((char *)row->content);
		if (!grid[j])
		{
			free_grid(grid, j);
			return (NULL);
		}
		row = row->next;
		j++;
	}
	map->width = (int)ft_strlen(grid[0]);
	return (grid);
}

t_map	load_map(const char *file_path)
{
	int		fd;
	t_list	*rows;
	t_map	map;

	map.height = 0;
	map.width = 0;
	map.grid = NULL;
	if (!file_path)
		error_exit(ERR_MAP_OPEN);
	fd = open(file_path, O_RDONLY);
	if (fd < 0)
		error_exit(ERR_MAP_OPEN);
	rows = get_rows(fd);
	close(fd);
	if (!rows)
		error_exit(ERR_MAP_FORMAT);
	map.grid = get_grid(rows, &map);
	free_rows(rows, 1);
	if (!map.grid)
		error_exit(ERR_MALLOC);
	return (map);
}
