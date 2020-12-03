#include "window.hpp"
#include <ncurses.h>
#include <string>
// initial drawing coordinates and dimensions for lines
constexpr int hline_starty = 6;
constexpr int hlline_startx = 1;
constexpr int hline_length = 12;

// vertical is roughly half of horizontal space in the terminals
constexpr int vline_starty = 1;
constexpr int vline_startx = 12;
constexpr int vline_length = 6;

struct position {
    position(int y, int x) : y(y), x(x) {};
    int y, x;
};

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

    position cursor = position(1,1);

    void render();
    void game_loop();
    void process_input(int input);

};

void sudoku_game::game_loop()
{
    while (is_running) {
        process_input(getch());
        render();
    }

}

void sudoku_game::process_input(int input) {
    switch(input) {
        case KEY_UP:
            cursor = position(cursor.y - 2, cursor.x);
            break;
        case KEY_RIGHT:
            cursor = position(cursor.y, cursor.x + 4);
            break;
        case KEY_LEFT:
            cursor = position(cursor.y, cursor.x - 4);
            break;
        case KEY_DOWN:
            cursor = position(cursor.y + 2, cursor.x);
            break;
        default:
            break;
    }
}

void sudoku_game::render()
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
    main_win.move_cursor(cursor.y, cursor.x);
}
