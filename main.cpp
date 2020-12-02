#include "screen.hpp"
#include "window.hpp"
#include <ncurses.h>

int main()
{
    // cpp wrapper class around ncurses screen logic
    auto screen = curses::screen();
    // custom template class that calls refresh on the screen when it falls out of scope
    curses::refresh_guard<curses::screen> screen_refresh(screen);

    curses::window section_1(6, 12, 0, 12);
    curses::window section_2(6, 12, 0, 24);
    curses::window section_3(6, 12, 0, 36);
    while (true)
    {
        curses::refresh_guard<curses::window> refresh1(section_1);
        curses::refresh_guard<curses::window> refresh2(section_2);
        curses::refresh_guard<curses::window> refresh3(section_3);
    }
    return 0;
}


