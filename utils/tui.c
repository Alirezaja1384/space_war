#include <ncurses.h>
#include <stdbool.h>
#include <assert.h>
#include "tui.h"
#include "colors.h"
#include "assertion.h"

WINDOW *mainwin = NULL;

WINDOW *setup_tui(void)
{
    WINDOW *scr = initscr();
    check(scr != NULL, "Unable to initialize screen");

    noecho();
    intrflush(stdscr, false);
    assert(curs_set(0) != ERR);
    start_color();
    register_colors();
    refresh();

    init_mainwin();
    keypad(mainwin, true);
    wrefresh(mainwin);
    wtimeout(mainwin, 100);
    wrefresh(mainwin);

    return mainwin;
};

void destroy_tui(void)
{
    endwin();
}

WINDOW *init_mainwin(void)
{
    mainwin = newwin(MW_HEIGHT, MW_WIDTH, Y_START, X_START);
    check(mainwin != NULL, "Unable to initialize main window");
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
