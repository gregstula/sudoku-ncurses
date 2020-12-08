// Gregory D. Stula
// CS325 Fall 2020
// Portfolio Project
#pragma once
#include <ncurses.h>

#include <string>

#include "screen.hpp"

namespace curses {
struct window {
    // constructor
    window(int height, int width, int startx, int starty, int cursor_setting = 0) noexcept
    {
        _win = newwin(height, width, startx, starty);
        // set cursor to visible, invisible, or super visible
        if (cursor_setting >= 0 && cursor_setting <= 2) {
            curs_set(cursor_setting);
        } else {
            curs_set(0);
        }
    }

    // default constructor
    window() noexcept { _win = nullptr; }

    // destructor
    // deletes window pointer
    ~window() noexcept
    {
        wclear(_win);
        wrefresh(_win);
        delwin(_win);
    }

    // passthrough functions to ncurses api
    // not all were used
    void clear() noexcept { werase(_win); }

    void clear_from_cursor_to_bottom() noexcept { wclrtobot(_win); }

    void clear_from_cursor_to_eol() noexcept { wclrtoeol(_win); }

    void refresh() noexcept { wrefresh(_win); }

    // prints string at coords
    // boolean for highlighting printed string
    template <class S>
    void print_at_coords(int y, int x, S&& str, bool attr = false)
    {
        if (attr) wattron(_win, A_STANDOUT);
        std::string s { std::forward<S>(str) };
        mvwprintw(_win, y, x, s.c_str());
        if (attr) wattroff(_win, A_STANDOUT);
    }

    // prints string at cursor, not used in sudoku
    template <class S>
    void print_at_cursor(S&& str, bool attr = false)
    {
        if (attr) wattron(_win, A_STANDOUT);
        std::string s { std::forward<S>(str) };
        wprintw(_win, s.c_str());
        if (attr) wattroff(_win, A_STANDOUT);
    }

    // moves cursor to position
    void move_cursor(int y, int x)
    {
        wmove(_win, y, x);
    }

    // gets value under the cursor
    char peek()
    {
        return winch(_win);
    }

    // prints horizontal line
    void print_hline(int y, int x, int length)
    {
        mvwhline(_win, y, x, '_', length);
    }

    // prints vertical line
    void print_vline(int y, int x, int length)
    {
        mvwvline(_win, y, x, '|', length);
    }

    void print_border() noexcept
    {
        wborder(_win, '|', '|', '-', '-', '+', '+', '+', '+');
    }

// ncurses window pointer
private:
    WINDOW* _win;
};

} // namespace curses
