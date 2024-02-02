#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include <unistd.h>
#include "assertion.h"
#include "colors.h"
#include "tui.h"

// NOTE: Since assertion can be disabled, they're not a
//  reliable solution for important checks.
void check(bool cond, char *message)
{
    if (!cond)
        error(message);
}

void error(char *message)
{
    WINDOW *win = get_win();

    wclear(win);
    wattr_on(win, COLOR_PAIR(CP_RED_BLACK), NULL);
    wprintw(win, "ERROR: %s\n", message);
    wattr_off(win, COLOR_PAIR(CP_RED_BLACK), NULL);
    wrefresh(win);
    sleep(5);

    destroy_tui();
    exit(1);
}
