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

    // original puzzle, hard coded
    std::array<std::array<char,9>, 9> puzzle = {{

{{'1', '2', '.', '4', '5', '.', '.', '.', '9'}},
{{'1', '2', '3', '4', '5', '.', '.', '8', '9'}},
{{'1', '2', '3', '4', '5', '6', '7', '8', '.'}},
{{'1', '2', '.', '4', '5', '6', '7', '8', '9'}},
{{'1', '2', '3', '4', '.', '6', '7', '8', '9'}},
{{'1', '2', '.', '4', '5', '6', '7', '8', '9'}},
{{'1', '2', '3', '4', '5', '6', '7', '8', '9'}},
{{'1', '2', '3', '.', '5', '6', '7', '8', '9'}},
{{'1', '2', '3', '4', '5', '6', '7', '8', '9'}},
    }};

    // in game puzzle to be modified
    std::array<std::array<char,9>, 9> game_puzzle = puzzle;

    char peeked = '.';
    int inserted = '.';
    curses::window main_win = curses::window(window_height, window_width,0,0);
    bool is_running = true;
    bool first_draw = true;

    coords cursor = coords(1,1);

    // game loop methods
    void game_loop();
    void process_input(int input);
    void update();
    void render();
    bool input_valid();
    coords convert_to_array_index(coords p);

};

// runs the main game loop functions
void sudoku_game::game_loop()
{
    while (is_running) {
        process_input(getch());
        update();
        render();
    }

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
    return coords ((p.y - window_start)/cur_y, (p.x - window_start)/cur_x);
}

void sudoku_game::update()
{
    // loop cursor around the window borders
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

    // get character under the cursor
    peeked = main_win.peek();
    if (peeked == '.' && input_valid()) {
        // convert current cursor coords to the relevant array index
        auto array_pos = convert_to_array_index(cursor);
        // insert the valid char in the game_array
        // we maintain the original puzzle array for tracking
        // which characters were inserted vs originally listed as clues
        game_puzzle[array_pos.x][array_pos.y] = inserted;
    }
}

// process input from keyboard
void sudoku_game::process_input(int input) {
    switch(input) {
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
        default:
            // all other input stored for validation
            inserted = input;
            break;
    }
}

void sudoku_game::render()
{
     // auto ncurses refresh via destructor
     curses::refresh_guard<curses::window> refresh1(main_win);

     /* manual effort to find the pattern
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
                // if the character is editable print it from the game board highlighted
                // this is so the user knows which numbers are their inputs vs puzzle clues
                main_win.print_at_coords((j * cur_y) + window_start, (i * cur_x) + window_start, std::string(1,game_puzzle[i][j]), true);
            } else {
                // otherwise print the clues as regular test
                main_win.print_at_coords((j * cur_y) + window_start, (i * cur_x) + window_start, std::string(1,puzzle[i][j]));
            }
        }
    }
    main_win.move_cursor(cursor.y, cursor.x);
}
