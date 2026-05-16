#ifndef SO_LONG_H
# define SO_LONG_H

# include "mlx.h"
# include "libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

# define IMG_SIZE 64

/******************* DATA STRUCTURES: *****************************************/
typedef enum e_error
{
	ERR_ARGUMENTS = 1,
	ERR_FILE_EXTENSION,
	ERR_MAP_OPEN,
	ERR_MAP_FORMAT,
	ERR_MAP_PATH,
	ERR_MALLOC,
	ERR_MLX
}	t_error;

typedef struct s_map
{
	int		width;
	int		height;
	char	**grid;
}	t_map;

typedef struct s_path
{
	int		found_c;
	int		found_e;
	char	**grid;
	t_map	*map;
}	t_path;

typedef struct s_image
{
	int		width;
	int		height;	
	void	*ptr;
}	t_image;

typedef struct s_game
{
	int		p_i;
	int		p_j;
	int		c_total;
	int		c_count;
	int		mv_count;
	void	*mlx;
	void	*win;
	t_map	map;
	t_image	w;
	t_image	f;
	t_image	e;
	t_image	p;
	t_image	c;
}	t_game;

/******************* ERROR UTILS: *********************************************/
void	error_exit(t_error status);

/******************* FREE UTILS: **********************************************/
void	free_grid(char **row, int count);
void	free_rows(t_list *row, int content);
void	free_map(t_map *map);
void	destroy_images(t_game *game);

/******************* MAP UTILS: ***********************************************/
t_map	load_map(const char *file_path);

/******************* FORMAT VALIDATION UTILS: *********************************/
void	validate_map_format(t_map *map);

/******************* PATH VALIDATION UTILS: ***********************************/
void	validate_map_path(t_map *map);

/******************* MLX GAME UTILS: ******************************************/
int		handle_key(int key, void *param);
int		handle_close(t_game *game);
void	init_game(t_game *game, t_map *map);
void	run_game(t_game *game);
void	render_game(t_game *game);

/******************* MOVEMENT UTILS: ******************************************/
int		next_blocked(t_game *game, int nxt_i, int nxt_j);
int		can_exit(t_game *game, char cell);
void	get_collectible(t_game *game, int nxt_i, int nxt_j);

#endif
