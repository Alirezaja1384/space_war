#ifndef sw_utils_form
#define sw_utils_form

#include <form.h>

#define TAB_KEY 9
#define SHIFT_TAB_KEY 353
#define SUBMIT_KEY '\n'

typedef void (*validation_error_handler)(FORM *, FIELD *);

bool form_handle_keys(FORM *form, int key, validation_error_handler error_handler);

#endif