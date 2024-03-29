#include <form.h>
#include <stdbool.h>
#include <ncurses.h>

#include "assertion.h"
#include "colors.h"
#include "form.h"

static bool validate_form(FORM *form, validation_error_handler error_handler)
{
    bool is_valid = true;
    int last_res = E_NO_MATCH;
    FIELD *initial_field = current_field(form);

    form_driver(form, REQ_FIRST_FIELD);
    for (int i = 0; i < field_count(form); i++)
    {
        is_valid = is_valid && (last_res = form_driver(form, REQ_VALIDATION)) == E_OK;
        if (is_valid)
            form_driver(form, REQ_NEXT_FIELD);
        else
            break;
    }

    if (is_valid)
    {
        // Prevent automatically focusing on last input while switching between fields
        set_current_field(form, initial_field);
        return true;
    }

    char *message = NULL;
    switch (last_res)
    {
    case E_INVALID_FIELD:
        message = "-- Invalid --";
        break;

    default:
        message = "Unknown error";
        break;
    }

    FIELD *field = current_field(form);
    set_field_buffer(field, 0, message);
    form_driver(form, REQ_END_LINE);

    if (error_handler != NULL)
        error_handler(form, field);

    return false;
}

bool form_handle_keys(FORM *form, int key, validation_error_handler error_handler)
{
    bool submit = false;
    switch (key)
    {
    case SUBMIT_KEY:
        // Sync the current field buffer with what is displayed
        // form_driver(form, REQ_NEXT_FIELD);
        // form_driver(form, REQ_PREV_FIELD);
        submit = validate_form(form, error_handler);

    case TAB_KEY:
    case KEY_DOWN:
        if (validate_form(form, error_handler))
        {
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
        }
        break;

    case SHIFT_TAB_KEY:
    case KEY_UP:
        if (validate_form(form, error_handler))
        {
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_END_LINE);
        }
        break;

    case KEY_LEFT:
        form_driver(form, REQ_PREV_CHAR);
        break;

    case KEY_RIGHT:
        form_driver(form, REQ_NEXT_CHAR);
        break;

    // Delete the char before cursor
    case KEY_BACKSPACE:
    case 127:
        form_driver(form, REQ_DEL_PREV);
        break;

    // Delete the char under the cursor
    case KEY_DC:
        form_driver(form, REQ_DEL_CHAR);
        break;

    default:
        form_driver(form, key);
        break;
    }

    return submit;
};

FIELD *label_field(char *text, int height, int width, int y_start, int x_start)
{

    FIELD *label = not_null(new_field(height, width, y_start, x_start, 0, 0));
    set_field_buffer(label, 0, text);
    set_field_opts(label, O_VISIBLE | O_PUBLIC | O_AUTOSKIP);

    return label;
}

void apply_modal_style(WINDOW *win, ColorPair cp, char *title)
{
    box(win, 0, 0);
    wbkgd(win, COLOR_PAIR(cp));
    mvwprintw(win, 0, 2, " %s ", title);
    mvwprintw(win, getmaxy(win) - 1, getmaxx(win) / 2 - 13, " Press 'Enter' to submit ");
}