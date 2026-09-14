#ifndef SO_LONG_H
# define SO_LONG_H

# include "mlx.h"
# include "libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

# define IMG_SIZE 64

/**
 * @file so_long.h
 * @brief Shared types and first-party interfaces for the so_long game.
 *
 * The project loads and validates a .ber map, initializes MiniLibX resources,
 * renders the map as a tiled 2D scene, and tracks player progression until
 * every collectible has been collected and the exit is reached.
 */

/**
 * @brief Terminal error codes used by the program.
 *
 * Each value identifies one user-visible failure category and is also used as
 * the process exit status when error_exit() terminates the program.
 */
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

/**
 * @brief Owns the loaded rectangular map grid.
 *
 * `grid` contains `height` allocated rows. A valid map uses `width`
 * characters per row. Ownership can be transferred into t_game during game
 * initialization.
 */
typedef struct s_map
{
	int		width;
	int		height;
	char	**grid;
}	t_map;

/**
 * @brief Temporary state used while validating map reachability.
 *
 * `grid` is an independent copy used by the flood-fill traversal so the
 * original map is not modified. `found_c` and `found_e` track reachable
 * collectibles and the exit.
 */
typedef struct s_path
{
	int		found_c;
	int		found_e;
	char	**grid;
	t_map	*map;
}	t_path;

/**
 * @brief Wraps a MiniLibX image and its dimensions.
 */
typedef struct s_image
{
	int		width;
	int		height;
	void	*ptr;
}	t_image;

/**
 * @brief Owns the runtime state and graphical resources of the game.
 *
 * The structure stores player coordinates, collectible progress, movement
 * count, MiniLibX handles, the owned map, and the five loaded tile images.
 */
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

/**
 * @brief Print the diagnostic for an error code and terminate the program.
 *
 * @param status Error category and process exit status.
 */
void	error_exit(t_error status);

/**
 * @brief Free an allocated grid prefix and its outer pointer array.
 *
 * @param row Grid to release.
 * @param count Number of allocated rows that can safely be freed.
 */
void	free_grid(char **row, int count);

/**
 * @brief Free a linked list used while assembling map rows.
 *
 * @param row First list node.
 * @param content Non-zero when each node's content must also be freed.
 */
void	free_rows(t_list *row, int content);

/**
 * @brief Release an owned map grid and reset its metadata.
 *
 * @param map Map whose allocated grid should be released.
 */
void	free_map(t_map *map);

/**
 * @brief Destroy every MiniLibX image currently owned by the game.
 *
 * Null image pointers are ignored.
 *
 * @param game Game containing the image handles.
 */
void	destroy_images(t_game *game);

/**
 * @brief Load a .ber file into an owned map structure.
 *
 * Fatal file and allocation failures terminate through error_exit().
 *
 * @param file_path Path to the map file.
 * @return Loaded map whose grid is owned by the caller.
 */
t_map	load_map(const char *file_path);

/**
 * @brief Validate the structural and character rules of a loaded map.
 *
 * Validation covers dimensions, rectangular shape, enclosing walls, accepted
 * symbols, and the required player, exit, and collectible counts.
 *
 * @param map Map to validate.
 */
void	validate_map_format(t_map *map);

/**
 * @brief Validate that required map targets are reachable from the player.
 *
 * Reachability is checked on an independent grid copy using flood-fill.
 *
 * @param map Previously format-validated map.
 */
void	validate_map_path(t_map *map);

/**
 * @brief Handle a MiniLibX keyboard event.
 *
 * Supports WASD, arrow keys, and Escape.
 *
 * @param key MiniLibX/X11 key value.
 * @param param Pointer to the active t_game.
 * @return Zero, as required by the MiniLibX callback interface.
 */
int		handle_key(int key, void *param);

/**
 * @brief Release graphical/runtime resources and terminate successfully.
 *
 * This is a terminal callback used by Escape, the window close event, and
 * successful completion of the game.
 *
 * @param game Active game state.
 * @return This function terminates the process and does not normally return.
 */
int		handle_close(t_game *game);

/**
 * @brief Initialize runtime state and graphical resources.
 *
 * Ownership of `map->grid` is transferred into `game->map`. The source map is
 * reset so only the game owns the grid after initialization.
 *
 * @param game Destination game state.
 * @param map Validated map whose ownership is transferred.
 */
void	init_game(t_game *game, t_map *map);

/**
 * @brief Render the initial frame, register event hooks, and enter the MLX loop.
 *
 * @param game Initialized game state.
 */
void	run_game(t_game *game);

/**
 * @brief Render the complete map and player at the current position.
 *
 * @param game Active game state.
 */
void	render_game(t_game *game);

/**
 * @brief Determine whether a target position cannot be entered.
 *
 * Positions outside the map and wall cells are blocked.
 *
 * @param game Active game state.
 * @param nxt_i Target horizontal map coordinate.
 * @param nxt_j Target vertical map coordinate.
 * @return Non-zero when movement is blocked, otherwise zero.
 */
int		next_blocked(t_game *game, int nxt_i, int nxt_j);

/**
 * @brief Determine whether the player may finish on the target cell.
 *
 * The exit is usable only when the target cell is `E` and every collectible
 * has already been collected.
 *
 * @param game Active game state.
 * @param cell Character stored in the target map cell.
 * @return Non-zero when the exit can be used, otherwise zero.
 */
int		can_exit(t_game *game, char cell);

/**
 * @brief Mark a collectible as collected.
 *
 * The collectible counter is incremented and the map cell becomes floor so it
 * cannot be collected a second time.
 *
 * @param game Active game state.
 * @param nxt_i Collectible horizontal coordinate.
 * @param nxt_j Collectible vertical coordinate.
 */
void	get_collectible(t_game *game, int nxt_i, int nxt_j);

#endif
