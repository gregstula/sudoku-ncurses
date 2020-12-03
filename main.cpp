// classes I wrote to wrap the relevant parts of ncurses in a C++ API
#include "screen.hpp"
#include "window.hpp"
#include "sudoku_game.hpp"

int main()
{
    // cpp wrapper class around ncurses screen logic
    auto screen = curses::screen();
    // custom template class the destructor calls refresh on the screen when the gaurd falls out of scope
    curses::refresh_guard<curses::screen> screen_refresh(screen);
    auto game = sudoku_game();
    game.render();
   return 0;
}

