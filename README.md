# so_long

## Description

**so_long** is a small 2D top‑down game developed as part of the 42 Common Core.  
The player moves around a tile‑based map, collecting all collectibles and then reaching
the exit to escape.

The map is described in a `.ber` file and must obey the following rules:

- The map is rectangular.
- Valid characters:
  - `1` – wall
  - `0` – empty / floor
  - `C` – collectible
  - `E` – exit
  - `P` – player start
- The map must:
  - Be fully surrounded by walls (`1` on the borders).
  - Contain **exactly 1** player start (`P`).
  - Contain **exactly 1** exit (`E`).
  - Contain **at least 1** collectible (`C`).
- There must exist a valid path from `P` to **all** collectibles and to the exit.

The project uses **MiniLibX** to open a graphical window, load XPM textures, and render
the game in a simple top‑down view.

## Instructions

### Requirements

The project is intended to be built and run on a Linux environment compatible with the
school’s configuration (X11):

- C compiler (`cc`)
- MiniLibX (provided in the `minilibx-linux` directory)
- X11 development libraries (typically installed on the school machines)

The project also uses a local copy of `libft` (provided in the `libft` directory).

### Compilation

From the project root:

```bash
make
```

This will:

- Compile `libft` using its own Makefile (bonus rule).
- Compile MiniLibX in `minilibx-linux`.
- Build the `so_long` executable with the required flags:
  - `-Wall -Wextra -Werror`
  - Linking with MiniLibX and X11 libraries.

To clean object files:

```bash
make clean
```

To remove objects and the executable:

```bash
make fclean
```

To rebuild everything from scratch:

```bash
make re
```

### Running

The program expects **exactly one** argument: the path to a valid `.ber` map file.

```bash
./so_long maps/example.ber
```

If:

- The number of arguments is incorrect,
- The file extension is not `.ber`,
- The file cannot be opened,
- The map format is invalid,
- Or no valid path exists between the player, collectibles and exit,

then the program prints:

```text
Error
[EXPLICIT ERROR MESSAGE]
```

and exits with a non‑zero status.

### Controls

- `W`, `A`, `S`, `D` or the arrow keys – move the player:
  - Up, left, down, right respectively.
- The player:
  - **Cannot** move through walls (`1`).
  - Can move freely across floor (`0`) and the exit (`E`).
  - Collects a collectible (`C`) by stepping on it.
- The move counter is printed to the shell on every valid move.
- When all collectibles have been gathered and the player steps on the exit (`E`),
  the game closes and the level is considered finished.
- `ESC` or clicking the window close button – exit the game cleanly.

## Project Structure

At a high level, the repository is organized as follows:

- `Makefile` – builds the project, libft and MiniLibX.
- `include/so_long.h` – main header, type definitions and prototypes.
- `src/` – source files for:
  - Argument and file handling
  - Map loading and parsing
  - Format validation and path validation (flood‑fill)
  - MiniLibX initialization, rendering and event loop
  - Player movement and move counter
  - Error and free utilities
- `libft/` – local copy of the libft project (including `ft_printf` and `get_next_line`).
- `minilibx-linux/` – MiniLibX sources and Makefile.
- `maps/` – example `.ber` maps.
- `textures/` – XPM textures for floor, wall, player, exit and collectibles.

## Resources

### Documentation & References

Some classic references and documentation used as general guidance for this project:

- 42's official **so_long** subject PDF.
- 42's **MiniLibX** documentation and examples.
- General C language references (e.g. man pages: `man 2 open`, `man 3 malloc`, etc.).
- Basic articles and references on:
  - Flood‑fill algorithms (for path validation).
  - Event‑driven programming and game loops.

### AI Usage

AI (ChatGPT) was used as a **support tool** during the development of this project, in particular to:

- Discuss the overall architecture of the project:
- Assist in refactoring functions to:
  - Respect the 42 Norm (function length, number of parameters, file organization).
  - Improve error handling and memory cleanup (centralizing free functions and MLX
    cleanup functions).

All AI‑assisted code and ideas were:

- Reviewed, understood, and adapted by me before being integrated.
- Checked against the project requirements and the 42 Norm.
- Tested (including with tools like `valgrind`) to ensure correctness and to avoid
  memory leaks.

No code was blindly copy‑pasted without verification. I remain fully responsible for
the final implementation and my understanding of every part of the project.
