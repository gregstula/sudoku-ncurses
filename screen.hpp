// Gregory D. Stula
// CS 325 Fall 2020
// Portfolio Project
#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <ncurses.h>
#include <string>

namespace curses {

// Attatch a window to this template class to use
// RAII to refresh an ncurses screen automatically when it falls out of scopt
// also redraws the window border
template <class S>
struct refresh_guard {
    typedef S screen_type;

    inline explicit refresh_guard(screen_type& s) noexcept : screen(s)
    {
        screen.clear();
        screen.print_border();
    }

    inline ~refresh_guard() { screen.refresh(); }
    refresh_guard& operator=(refresh_guard const&) = delete;
    refresh_guard(refresh_guard const&) = delete;

private:
    screen_type& screen;
};

class screen {
public:
    // constructor, initialized ncurses with needed settings
    screen() noexcept
    {
        // ncurses settings
        setlocale(LC_ALL, "en_US.UTF-8");
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        meta(stdscr, TRUE);
        nodelay(stdscr, TRUE); // let getch pass through
        curs_set(1);
        typeahead(-1);
    }

    // destructor
    ~screen() noexcept
    {
        clear();
        endwin();
    }

    // gets char
    // window variant is used
    auto get_key() -> int { return getch(); }

    // prints on the screen as opposed to a window
    // not used
    template <typename S>
    void print_at_coords(int y, int x, S&& str)
    {
        std::string s { std::forward<S>(str) };
        mvprintw(y, x, s.c_str());
    }

    // prints on the screen as opposed to the window
    // not used
    template <typename S>
    void print_at_cursor(S&& str)
    {
        std::string s { std::forward<S>(str) };
        printw(s.c_str());
    }

    // dummy function, does nothing. for template
    void print_border() noexcept {}
    // clears the screen
    void clear() noexcept { ::clear(); }
    // refreshes the screen
    void refresh() noexcept { ::refresh(); }

private:
};

} // namespace curses
