# Sudoku

A Sudoku game supporting several grid size, programmed in C++ and QT.

## Tasks and Features

- [x] Grid generator
  - [x] Sizes
    - [x] 3x3 sub-grids
    - [x] 4x4 sub-grids
    - [x] ~~5x5 sub-grids~~
  - [x] Difficulties
    - [x] Easy
    - [x] Medium
    - [x] Hard
- [x] Game
  - [x] New game
    - [x] Select grid size (3x3, 4x4 or ~~5x5~~)
    - [x] Select difficulty (easy, medium, hard)
    - [x] Select number of players
    - [x] Generate new grid
  - [x] Play
    - [x] Number entry
    - [x] Entry validation
    - [x] Change text color according to active player
    - [x] Finish player's turn when incorrect answer given (invalid input doesn't count e.g: letters)
    - [ ] Finish player's turn when give answer already suggested before
  - [x] Victory : Dialog
- [ ] Scoreboard
  - [ ] Calculate score
  - [ ] save scores of players
  - [ ] display the 10 best players

## Compiling

### Install dependencies

**Fedora:**

```
sudo dnf install qt6-qtbase-devel
```

### Meson

1. build: `meson setup build && cd build`
2. compile: `meson compile`
3. run: `./src/sudoku-qt`

### CMake

1. build:
  1. `mkdir build`
  2. `cd build`
  3. `cmake ..`
2. compile: `make`
3. run: `./sudoku-qt`
