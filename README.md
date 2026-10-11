# Brick by Brick

A Breakout-style game written in C++17 with a gtkmm 4 graphical interface, built as the semester project of the EPFL programming course (2026).

![Gameplay](screenshot.png)

## Features

- **Physics simulation**: ball/brick and ball/paddle collisions, computed step by step at a fixed time interval (25 ms)
- **Three brick types** built on an abstract `Brick` class with polymorphism: rainbow bricks that take up to 7 hits (their color shows the hits left), bricks that release a new ball and bricks that split into smaller ones
- **Save and load**: game states are read from and written to text files, with validation of the file content (malformed files are rejected with an explicit error)
- **Graphical interface**: open, save, restart, start/stop and step-by-step execution, with the paddle controlled by the mouse

## Build and run

Requirements: a C++17 compiler and gtkmm 4.0 (`pkg-config` must find `gtkmm-4.0`).

```bash
make                    # build the executable
./project tests/t00.txt # launch the game on a given state file
make tests              # run every scenario in tests/
make clean              # remove build files
```

## Controls

| Input        | Action             |
| ------------ | ------------------ |
| Mouse        | Move the paddle    |
| `s`          | Start / stop       |
| `1`          | Advance one step   |
| `r`          | Restart            |

## Project structure

| File                      | Role                                              |
| ------------------------- | ------------------------------------------------- |
| `project.cc`              | Entry point                                       |
| `gui.cc/.h`               | Window, buttons, mouse and keyboard handling      |
| `graphic.cc/.h`           | Drawing primitives                                |
| `game.cc/.h`              | Game state, simulation loop, file reading/saving  |
| `ball`, `brick`, `paddle` | Game objects and collision logic                  |
| `message.cc/.h`           | Error messages for invalid files                  |
| `tools.cc/.h`             | Geometry utilities                                |
| `tests/`                  | 20 game state files used as test scenarios        |

## Authors

Antoine Devilez and Gaspar Duarte Ribeiro, EPFL
