# CS 325 Fall 2020
##Gregroy D. Stula
##Final Project

**Final Project**
The project was tested on the flip engr server.
The project's GUI requires the ncurses library, which exists on the engr server. Linking is handled in the make file.

**Compilation Instructions**
To compile simply type `make` and allow g++ to compile with `g++ -std=c+11 -lncurses`.
Then run the executable by typing `./sudoku`

**Project Structure**
The project consists of 3 header files and a main.cpp file.
The first 2 header files are screen.hpp and window.hpp.
These header files abstract away some of ncurses C API and wrap them in a more C++ friendly interface.

**Game Controls**
- Press the 'q' key at anytime to quit
- Use the arrow keys to navigate the cursor around the board
- The directional movements will wrap around the board
- You can try a number at any cursior position that is not highlighted.
- Untried positions are marked as '.', these can be retried later
- You can automatically solve the game at any time by pressing the 's' key
