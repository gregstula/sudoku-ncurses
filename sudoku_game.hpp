// Gregory D Stula
// CS 325 Fall 2020
// Portfolio Project
#pragma once
#include "window.hpp"
#include <ncurses.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <unordered_set>
#include <iostream>

// initial drawing coordinates and dimensions for lines
constexpr int hline_starty = 6;
constexpr int hlline_startx = 1;
constexpr int hline_length = 12;

// vertical is roughly half of horizontal space in the terminals
constexpr int vline_starty = 1;
constexpr int vline_startx = 12;
constexpr int vline_length = 6;

// window dimensions
constexpr int window_start = 1;
constexpr int window_height = (vline_length * 3) + window_start;
constexpr int window_width = (hline_length * 3) + window_start;

// cursor movement offsets
constexpr int cur_x = 4;
constexpr int cur_y = 2;

struct coords {
    coords(int y, int x) : y(y), x(x) {};
    int y, x;
};

struct sudoku_game {

    void game_loop();
private:
    // original puzzle, hard coded
    // awkward syntax to work on flip g++ for C++11
    std::array<std::array<char, 9>, 9> puzzle = { {

        { { '8', '.', '.', '9', '3', '.', '.', '.', '2' } },
        { { '.', '.', '9', '.', '.', '.', '.', '4', '.' } },
        { { '7', '.', '2', '1', '.', '.', '9', '6', '.' } },
        { { '2', '.', '.', '.', '.', '.', '.', '9', '.' } },
        { { '.', '6', '.', '.', '.', '.', '.', '7', '.' } },
        { { '.', '7', '.', '.', '.', '6', '.', '.', '5' } },
        { { '.', '2', '7', '.', '.', '8', '4', '.', '6' } },
        { { '.', '3', '.', '.', '.', '.', '5', '.', '.' } },
        { { '5', '.', '.', '.', '6', '2', '.', '.', '8' } },
    } };

    // in game puzzle to be modified
    std::array<std::array<char, 9>, 9> game_puzzle = puzzle;

    // value read under cursor
    char peeked = '.';

    // value to be inserted at cursor
    int inserted = '.';

    // main ncurses window
    curses::window main_win = curses::window(window_height + 4, window_width, 0, 0, 1);

    // game is running
    bool is_running = true;

    // cursor position
    coords cursor = coords(1, 1);

    // game loop methods
    void process_input(int input);
    void update();
    void render();
    bool input_valid();
    void end_game();
    int spaces_left();
    coords convert_to_array_index(coords p);

    // validate solution
    bool col_is_valid(int col);
    bool row_is_valid(int row);
    bool grid_is_valid(int start_row, int start_col);
    bool board_is_valid();

    // generate solution
    bool row_legal(int row, char val);
    bool col_legal(int col, char val);
    bool grid_legal(int start_row, int start_col, char val);
    bool is_legal(int row, int col, char val);
    bool find_editable_location(int& row, int& col);
    bool generate_solution();
};

// flips game loop bool to quit the game
void sudoku_game::end_game()
{
    is_running = false;
}


// runs the main game loop functions
void sudoku_game::game_loop()
{
    while (is_running) {
        process_input(getch());
        update();
        render();
    }
}

// counts how many empty spaces are left
int sudoku_game::spaces_left()
{
    int count = 0;
    for (auto&& a : game_puzzle) {
        count += std::count_if(a.begin(), a.end(), [&](char& i) { return (i == '.'); });
    }
    return count;
}

// checks if the input acceptable
bool sudoku_game::input_valid()
{
    // convert char to be inserted into an int
    // and check that it is in the range 1 - 9
    int attempt = inserted - '0';
    if (attempt > 0 && attempt <= 9) {
        return true;
    }
    return false;
}

// convert the render coordinated back to array coordinates
coords sudoku_game::convert_to_array_index(coords p)
{
    return coords((p.y - window_start) / cur_y, (p.x - window_start) / cur_x);
}

// updates game logic relative to cursor position
void sudoku_game::update()
{
    // check if cursor is in a valid position
    // loops cursor around the window borders
    if (cursor.x >= window_width) {
        cursor = coords(cursor.y, window_start);
    }
    else if (cursor.x < window_start) {
        cursor = coords(cursor.y, window_width - cur_x);
    }
    else if (cursor.y >= window_height) {
        cursor = coords(window_start, cursor.x);
    }
    else if (cursor.y < window_start) {
        cursor = coords(window_height - cur_y, cursor.x);
    }

    // convert current cursor coords to the relevant array index
    auto array_pos = convert_to_array_index(cursor);
    // check if this was an editable section in the original puzzle
    if (puzzle[array_pos.y][array_pos.x] == '.' && input_valid()) {
       // insert the valid char in the game_array
        // we maintain the original puzzle array for tracking
        // which characters were inserted vs originally listed as clues
        game_puzzle[array_pos.y][array_pos.x] = inserted;
    }
}

// process input from keyboard
void sudoku_game::process_input(int input)
{
    switch (input) {
    // handle directional input, update cursor coords for rendering
    case KEY_UP:
        cursor = coords(cursor.y - cur_y, cursor.x);
        break;
    case KEY_RIGHT:
        cursor = coords(cursor.y, cursor.x + cur_x);
        break;
    case KEY_LEFT:
        cursor = coords(cursor.y, cursor.x - cur_x);
        break;
    case KEY_DOWN:
        cursor = coords(cursor.y + cur_y, cursor.x);
        break;
    // quit game on q press
    case 'q':
        end_game();
        break;
    // solve game on s press
    case 's':
        // reset game puzzle
        game_puzzle = puzzle;
        // solve
        generate_solution();
        break;
    default:
        // all other input stored for validation
        inserted = input;
        break;
    }
}

// draw the game
void sudoku_game::render()
{
    // auto ncurses refresh via destructor
    curses::refresh_guard<curses::window> refresh1(main_win);

    /* manual effort used to find the pattern
        main_win.print_hline(6, 1, 12);
        main_win.print_vline(1,12, 6);
        main_win.print_hline(6, 13, 12);
        main_win.print_vline( 1, 24, 6);
        main_win.print_hline(6, 25, 12);
        main_win.print_vline( 1, 36, 6);
        main_win.print_hline(12, 1, 12);
        main_win.print_vline( 7, 12, 6);
    */

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            main_win.print_hline(hline_starty + (hline_starty * i), hlline_startx + (hline_length * j), hline_length);
            main_win.print_vline(vline_starty + (vline_length * i), vline_startx + (vline_startx * j), vline_length);
        }
    }

    // print puzzle
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            // print api only accepts strings currently
            // boolean at the end represents whether to print highlighted or not
            if (puzzle[i][j] == '.') {
                // if the character is editable print it from the game board non-highlighted
                // this is so the user knows which numbers are their inputs vs puzzle clues
                main_win.print_at_coords((i * cur_y) + window_start, (j * cur_x) + window_start, std::string(1, game_puzzle[i][j]));
            }
            else {
                // otherwise print the clues as regular test
                // regular clues are printed highlighted so they can be differentiated by the user
                main_win.print_at_coords((i * cur_y) + window_start, (j * cur_x) + window_start, std::string(1, puzzle[i][j]), true);
            }
        }
    }
    // print puzzle status to user
    std::stringstream ss;
    // create string with spaces left and print it
    ss << "Spaces left: " << spaces_left();
    if (spaces_left() != 0) {
        ss << " | Unsolved.";
    } else {
        if (board_is_valid()) {
            ss << " | SOLVED!";
        } else {
            ss << " | INVALID!";
        }
    }
    main_win.print_at_coords(window_height + window_start, window_start, ss.str());

    // move cursor to correct position
    main_win.move_cursor(cursor.y, cursor.x);
}


// checks if value can be legally inserted to row
// returns false if move is illegal
bool sudoku_game::row_legal(int row, char val) {
    for (int i = 0; i < 9; i++) {
        if (game_puzzle[row][i] == val) {
            return false;
        }
    }
    return true;
}


// checks if value can be legally inserted to col
// returns false if move is illegal
bool sudoku_game::col_legal(int col, char val) {
    for (int i = 0; i < 9; i++) {
        if (game_puzzle[i][col] == val) {
            return false;
        }
    }
    return true;

}


// checks if value can be inserted in to 3x3 grid
// returns false if move is illegal
bool sudoku_game::grid_legal(int start_row, int start_col, char val) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (game_puzzle[start_row + i][start_col + j] == val) {
                return false;
            }
        }
    }
    return true;
}

// utility function that checks if move is legal for  rows cols and 3x3 grids
bool sudoku_game::is_legal(int row, int col, char val) {
    return row_legal(row, val) && col_legal(col, val) && grid_legal( row - row % 3, col - col % 3, val) && game_puzzle[row][col] == '.';
}


// takes two integers by reference and returns the coord
// of the next editable char and returns true if found, false otherwise
bool sudoku_game::find_editable_location(int& row, int& col) {
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (game_puzzle[row][col] == '.') {
                return true;
            }
        }
    }
    return false;
}


// recursive backtracking algorithm that brute forces the solution
bool sudoku_game::generate_solution() {

    // find editable location coords by reference
    int row, col;
    // game is solved if all positions are filled
    if (!find_editable_location(row, col)) return true;

    // genrate chars 1-9 to test
    for (char val = '1'; val <= '9'; val++) {
        // try num in position if move is legal
        if (is_legal(row, col, val)) {
            game_puzzle[row][col] = val;
            // check if solved
            if(generate_solution()) {
                return true;
            }
            // reset to retry if move didn't work
            game_puzzle[row][col] = '.';
        }
    }
    return false;
}

// checks if given column is valid
bool sudoku_game::col_is_valid(int col)
{
    std::unordered_set<char> unique;
    for (int i = 0; i < 9; i++) {
        // all the values in a set are unique if insetion fails
        // we have dupliucate values in the column
        // and the soduku solution is invalid
        if (!unique.insert(game_puzzle[i][col]).second) {
            return false;
        }
   }
   return true;
}

// checks if given row is valid
bool sudoku_game::row_is_valid(int row)
{
    std::unordered_set<char> unique;
    for (int i = 0; i < 9; i++) {
        // all the values in a set are unique, so if insetion fails
        // we have dupliucate values in the row
        // and the soduku solution is invalid
        if (!unique.insert(game_puzzle[row][i]).second) {
            return false;
        }
   }
   return true;
}

// checks if 3x3 grid is valid
bool sudoku_game::grid_is_valid(int start_row, int start_col)
{
    std::unordered_set<char> unique;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // all the values in a set are unique, so if insetion fails
            // we have dupliucate values in the column
            // and the soduku solution is grid
            if (!unique.insert(game_puzzle[start_row + i][start_col + j]).second) {
                return false;
            }

        }
    }
    return true;
}

// checks if all cols, rows, and subgrids are valid
bool sudoku_game::board_is_valid()
{
    // if row or column is not valid then board is invalid
    for (int i = 0; i < 9; i++) {
        if (!row_is_valid(i) && !col_is_valid(i)) {
            return false;
        }
        // check 3x3 grid at multiples of 3
        // if grid is invalid then the board is invalid
        if (i % 3 == 0) {
            for (int j = 0; j <= 6; j += 3) {
                if (!grid_is_valid(i, j)) {
                    return false;
                }
            }
        }
    }
    return true;
}


