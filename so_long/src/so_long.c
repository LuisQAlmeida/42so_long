#include "so_long.h"

static int	valid_file_ext(const char *file_path, const char *file_ext)
{
	size_t	path_length;
	size_t	ext_length;
	size_t	i;

	if (!file_path || !file_ext)
		return (0);
	i = 1;
	ext_length = ft_strlen(file_ext);
	path_length = ft_strlen(file_path);
	if (path_length < ext_length)
		return (0);
	while (i <= ext_length)
	{
		if (file_path[path_length - i] != file_ext[ext_length - i])
			return (0);
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_map	map;
	t_game	game;

	if (argc != 2)
		error_exit(ERR_ARGUMENTS);
	if (!valid_file_ext(argv[1], ".ber"))
		error_exit(ERR_FILE_EXTENSION);
	map = load_map(argv[1]);
	validate_map_format(&map);
	validate_map_path(&map);
	init_game(&game, &map);
	run_game(&game);
	return (0);
}
