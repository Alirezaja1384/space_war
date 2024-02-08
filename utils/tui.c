#include <locale.h>
#include <ncurses.h>
#include <stdbool.h>
#include <assert.h>
#include "tui.h"
#include "colors.h"
#include "assertion.h"

WINDOW *mainwin = NULL;

WINDOW *setup_tui(int height, int width)
{
    check(setlocale(LC_CTYPE, LOCALE) != NULL, "Unable to set locale to " LOCALE);

    WINDOW *scr = initscr();
    check(scr != NULL, "Unable to initialize screen");

    noecho();
    intrflush(stdscr, false);
    assert(curs_set(0) != ERR);
    start_color();
    register_colors();
    refresh();

    mainwin = init_mainwin(height, width);
    keypad(mainwin, true);
    wrefresh(mainwin);
    wtimeout(mainwin, NCURSES_TIMEOUT);
    wrefresh(mainwin);

    return mainwin;
};

void destroy_tui(void)
{
    endwin();
}

WINDOW *init_mainwin(int height, int width)
{
    WINDOW *win = newwin(height, width, (LINES - height) / 2, (COLS - width) / 2);
    check(win != NULL, "Unable to initialize main window");

    refresh();
    wrefresh(win);

    return win;
}

WINDOW *init_header_win(int mainwin_height, int mainwin_width)
{
    int height = (LINES - mainwin_height) / 2;
    int width = mainwin_width;

    int x_start = (COLS - width) / 2;

    WINDOW *win = newwin(height, width, 0, x_start);
    check(win != NULL, "Unable to initialize header window");

    refresh();
    wrefresh(win);

    return win;
}

void destroy_win(WINDOW *win)
{
    wclear(win);
    wrefresh(win);
    assert(delwin(win) != ERR);
    refresh();
}

WINDOW *get_mainwin(void)
{
    assert(mainwin != NULL);
    return mainwin;
}

WINDOW *get_win(void)
{
    return mainwin != NULL ? mainwin : stdscr;
}
