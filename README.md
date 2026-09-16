# so_long

[![Regression](https://github.com/LuisQAlmeida/42so_long/actions/workflows/regression.yml/badge.svg)](https://github.com/LuisQAlmeida/42so_long/actions/workflows/regression.yml)

> Part of my [42 Common Core portfolio](https://github.com/LuisQAlmeida/42Portfolio).

A small tile-based 2D game written in C with MiniLibX, focused on map parsing,
validation, path reachability, event-driven rendering, and explicit resource
ownership.

> **42 Common Core:** 100/100 · Subject v5.0 · [Academic record & evaluation](docs/academic/README.md)

## Table of Contents

- [Overview](#overview)
- [Controls](#controls)
- [Validation Pipeline](#validation-pipeline)
- [Architecture](#architecture)
- [Dependencies](#dependencies)
- [Build](#build)
- [Running](#running)
- [Testing](#testing)
- [Continuous Integration](#continuous-integration)
- [Quality and Robustness](#quality-and-robustness)
- [Project Structure](#project-structure)
- [AI Usage](#ai-usage)
- [License](#license)



## Overview

The goal is to navigate a map, collect every collectible, and reach the exit.

Maps are stored as `.ber` files and use five symbols:

| Symbol | Meaning |
| --- | --- |
| `0` | Empty floor |
| `1` | Wall |
| `C` | Collectible |
| `E` | Exit |
| `P` | Player start |

A valid map must:

- be rectangular;
- be fully enclosed by walls;
- contain exactly one player;
- contain exactly one exit;
- contain at least one collectible;
- use only the accepted map symbols;
- provide a valid path from the player to every collectible and the exit.

Reaching the exit completes the game only after all collectibles have been
collected.

## Controls

| Input | Action |
| --- | --- |
| `W` / `↑` | Move up |
| `A` / `←` | Move left |
| `S` / `↓` | Move down |
| `D` / `→` | Move right |
| `Esc` | Exit cleanly |
| Window close button | Exit cleanly |

Every successful movement is counted and printed to the terminal.

## Validation Pipeline

Before the graphical game loop starts, input passes through a staged validation
pipeline:

```text
command-line arguments
        ↓
file extension
        ↓
map loading
        ↓
format validation
        ↓
path reachability
        ↓
MiniLibX initialization
        ↓
event loop
```

### Format validation

The format layer checks:

- rectangular shape;
- minimum usable dimensions;
- enclosing walls;
- accepted characters;
- exactly one `P`;
- exactly one `E`;
- at least one `C`.

### Path validation

A copy of the map is traversed with recursive flood-fill from the player's
starting position.

The original grid is left unchanged while the traversal verifies that:

- every collectible is reachable;
- the exit is reachable.

The recursive implementation was stress-tested during the portfolio audit well
beyond practical rendered map sizes. It remains recursive by design and, like
other recursive traversals, is ultimately bounded by the process stack.

## Architecture

The first-party implementation is split by responsibility:

| File | Responsibility |
| --- | --- |
| `src/so_long.c` | Argument validation and top-level program flow |
| `src/map_utils.c` | File loading and construction of the map grid |
| `src/format_validation_utils.c` | Structural and character validation |
| `src/path_validation_utils.c` | Reachability validation with flood-fill |
| `src/game_init.c` | Runtime state, MiniLibX window, and texture initialization |
| `src/game_loop.c` | Input handling, movement flow, event loop, and shutdown |
| `src/game_render.c` | Tile and player rendering |
| `src/movement_utils.c` | Movement predicates and collectible handling |
| `src/free_utils.c` | Map, list, grid, and image cleanup |
| `src/error_utils.c` | Error diagnostics and terminal error exits |

Shared structures, ownership rules, callback contracts, and public first-party
interfaces are documented with Doxygen-style comments in
[`include/so_long.h`](include/so_long.h).

## Dependencies

The maintained repository uses two pinned Git submodules:

| Dependency | Path | Pinned revision |
| --- | --- | --- |
| Libft | `external/libft` | `0227823923ca15b580a481c3fb929d7f1382f545` |
| MiniLibX | `external/minilibx-linux` | `f07d00f07c5c652223f505b526f84dab73cf2598` |

Libft is pinned to the canonical
[`42Libft`](https://github.com/LuisQAlmeida/42Libft) release used by this
portfolio.

MiniLibX is pinned to a specific upstream revision rather than downloaded from
a mutable source during the build.

This makes the dependency state reproducible: Git determines the source
revision, while Make builds that pinned source.

## Build

### Requirements

The maintained version targets Linux with X11.

On Debian/Ubuntu systems, the required build packages can be installed with:

```bash
sudo apt install build-essential libx11-dev libxext-dev libbsd-dev
```

### Clone with dependencies

```bash
git clone --recurse-submodules git@github.com:LuisQAlmeida/42so_long.git
cd 42so_long
make
```

If the repository was cloned without its submodules:

```bash
git submodule update --init --recursive
```

Then build normally:

```bash
make
```

The Makefile builds the pinned MiniLibX and Libft dependencies before linking
the `so_long` executable.

Useful targets:

```bash
make
make clean
make fclean
make re
```

## Running

Run the program with exactly one `.ber` map:

```bash
./so_long maps/map_valid_subject_1.ber
```

Invalid arguments, files, map formats, paths, allocation failures, or graphical
initialization failures terminate with an explicit diagnostic beginning with:

```text
Error
```

and a non-zero exit status.

## Testing

The repository includes a deterministic headless regression suite:

```bash
./tests/run_regression.sh
```

The compiler can be selected explicitly:

```bash
CC=cc ./tests/run_regression.sh
CC=clang ./tests/run_regression.sh
```

The current suite contains **32 cases**:

| Category | Cases |
| --- | ---: |
| Argument / file handling | 4 |
| Format validation | 20 |
| Path validation | 2 |
| Valid maps reaching the graphical boundary | 6 |
| **Total** | **32** |

Valid maps are tested headlessly by running without a display. Reaching the
expected MiniLibX initialization failure proves that argument handling, map
loading, format validation, and path validation all succeeded before the
graphical boundary.

The repository fixtures currently contain:

```text
22 invalid maps
6 valid maps
```

## Continuous Integration

GitHub Actions runs the same canonical regression suite defined in
`tests/run_regression.sh`.

The workflow is stored at:

```text
.github/workflows/regression.yml
```

It runs on pull requests targeting `main` and on pushes to `main`, using both:

```text
cc
clang
```

The CI workflow deliberately invokes the repository test runner instead of
duplicating its test contract.

## Quality and Robustness

The portfolio modernization included targeted engineering audits rather than
only cosmetic repository changes.

Validated areas include:

- deterministic argument, format, path, and valid-map regression coverage;
- GCC-compatible `cc` and Clang builds;
- Valgrind checks for successful gameplay shutdown;
- Valgrind checks for `Esc` shutdown;
- Valgrind checks for window-close shutdown;
- explicit map file-descriptor ownership;
- cleanup of partial map-loading allocations;
- cleanup of path-validation copy allocations;
- controlled allocation-failure injection;
- shared image cleanup for terminal graphical paths;
- Makefile tracking of the shared project header;
- recursive flood-fill stress testing beyond practical rendered map sizes.

The normal graphical shutdown paths tested with Valgrind finish with no
first-party heap allocations remaining and no reported memory errors.

## Project Structure

```text
.
├── .github/
│   └── workflows/
│       └── regression.yml
├── docs/
│   ├── academic/
│   │   └── README.md
│   └── assets/
│       └── 42-evaluation.png
├── external/
│   ├── libft/
│   └── minilibx-linux/
├── include/
│   └── so_long.h
├── maps/
│   ├── map_invalid_*.ber
│   └── map_valid_*.ber
├── src/
│   ├── error_utils.c
│   ├── format_validation_utils.c
│   ├── free_utils.c
│   ├── game_init.c
│   ├── game_loop.c
│   ├── game_render.c
│   ├── map_utils.c
│   ├── movement_utils.c
│   ├── path_validation_utils.c
│   └── so_long.c
├── tests/
│   └── run_regression.sh
├── textures/
│   ├── collectible.xpm
│   ├── exit.xpm
│   ├── floor.xpm
│   ├── player.xpm
│   └── wall.xpm
├── .gitmodules
├── LICENSE
├── Makefile
└── README.md
```


## AI Usage

AI assistance was used transparently in two different phases of this project.

### Original academic development

During parts of the original project, ChatGPT was used as a support tool for
activities such as:

- discussing architecture and implementation decisions;
- reviewing and refactoring functions;
- checking understanding of the project requirements;
- reasoning about error handling and memory cleanup;
- reviewing tests and potential edge cases.

AI suggestions were treated as material to review rather than as authoritative
output. Proposed code and ideas were understood, adapted where appropriate,
and tested before integration.

### Portfolio modernization

AI was used more extensively as an engineering assistant during the later
professional modernization of the repository, including:

- systematic code and repository audits;
- dependency-migration planning;
- regression-test design;
- controlled allocation-failure experiments;
- Valgrind result analysis;
- CI design;
- Git and GitHub workflow planning;
- maintainability review;
- documentation and repository organization.

The resulting changes were reviewed against the actual source, validated
locally, and, where applicable, exercised by the regression suite and CI.

The purpose of documenting this is transparency about the development process,
not to minimize or exaggerate the role of AI.

## License

This repository is distributed under the [MIT License](LICENSE).
