#include "screen.hpp"
#include "window.hpp"
#include <ncurses.h>

int main()
{
    // cpp wrapper class around ncurses screen logic
    auto screen = curses::screen();
    // custom template class the destructor calls refresh on the screen when the gaurd falls out of scope
    curses::refresh_guard<curses::screen> screen_refresh(screen);

    curses::window section_1(40, 80, 0, 0);
    while (true)
    {
        curses::refresh_guard<curses::window> refresh1(section_1);
        section_1.print_hline(6,1,12);
        section_1.print_vline(1,12,6);
    }
    return 0;
}


