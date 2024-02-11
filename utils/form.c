#include <form.h>
#include <stdbool.h>
#include <ncurses.h>

#include "form.h"

static bool validate_form(FORM *form, validation_error_handler error_handler)
{
    int res = form_driver(form, REQ_VALIDATION);
    if (res == E_OK)
        return true;

    char *message = NULL;
    switch (res)
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
        if (validate_form, error_handler)
        {
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
        }
        break;

    case SHIFT_TAB_KEY:
    case KEY_UP:
        if (validate_form, error_handler)
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
