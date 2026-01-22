#include "so_long.h"

static const char	*get_message(t_error status)
{
	if (status == ERR_ARGUMENTS)
		return ("[INVALID ARGUMENTS]\n");
	if (status == ERR_FILE_EXTENSION)
		return ("[INVALID FILE EXTENSION]\n");
	if (status == ERR_MAP_OPEN)
		return ("[FAILED TO OPEN FILE]\n");
	if (status == ERR_MAP_FORMAT)
		return ("[INVALID MAP FORMAT]\n");
	if (status == ERR_MAP_PATH)
		return ("[INVALID MAP PATH]\n");
	if (status == ERR_MALLOC)
		return ("[FAILED TO ALLOCATE MEMORY]\n");
	if (status == ERR_MLX)
		return ("[FAILED TO INITIALIZE GRAPHICS]\n");
	return ("[UNKNOWN ERROR]");
}

void	error_exit(t_error status)
{
	const char	*message;

	message = get_message(status);
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd((char *)message, 2);
	exit((int)status);
}
