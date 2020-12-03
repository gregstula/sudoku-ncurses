#include "window.hpp"
#include <string>
// initial drawing coordinates and dimensions for lines
constexpr int hline_starty = 6;
constexpr int hlline_startx = 1;
constexpr int hline_length = 12;

// vertical is roughly half of horizontal space in the terminals
constexpr int vline_starty = 1;
constexpr int vline_startx = 12;
constexpr int vline_length = 6;

struct sudoku_game {

    char numbers[9][9] = {
        {'1', '2', '3', '4', '5', '6', '7', '8', '9'},
        {'1', '2', '3', '4', '5', '6', '7', '8', '9'},
        {'1', '2', '3', '4', '5', '6', '7', '8', '9'},
        {'1', '2', '3', '4', '5', '6', '7', '8', '9'},
        {'1', '2', '3', '4', '5', '6', '7', '8', '9'},
        {'1', '2', '3', '4', '5', '6', '7', '8', '9'},
        {'1', '2', '3', '4', '5', '6', '7', '8', '9'},
        {'1', '2', '3', '4', '5', '6', '7', '8', '9'},
        {'1', '2', '3', '4', '5', '6', '7', '8', '9'},
    };

    curses::window main_win = curses::window((vline_length * 3) + 1, (hline_length * 3) + 1,0,0);
    bool is_running = true;

    void render();

};

void sudoku_game::render() {

    while (is_running)
    {
        // auto ncurses refresh via destructor
        curses::refresh_guard<curses::window> refresh1(main_win);

        /*
        main_win.print_hline(6, 1, 12);
        main_win.print_vline(1,12, 6);
        main_win.print_hline(6, 13, 12);
        main_win.print_vline( 1, 24, 6);
        main_win.print_hline(6, 25, 12);
        main_win.print_vline( 1, 36, 6);
        main_win.print_hline(12, 1, 12);
        main_win.print_vline( 7, 12, 6);
        */



        // draw 9x9 grid

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                main_win.print_hline(hline_starty + (hline_starty * i), hlline_startx + (hline_length * j), hline_length);
                main_win.print_vline(vline_starty + (vline_length * i), vline_startx + (vline_startx * j), vline_length);
            }
        }

        // print numbers
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                main_win.print_at_coords((j * 2) + 1, (i * 4) + 1, std::string(1,numbers[i][j]));
            }
        }
    }
}
