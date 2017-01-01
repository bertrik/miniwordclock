#include <stdbool.h>
#include "editline.h"

#define BELL    0x07
#define BS      0x08
#define LF      0x0A
#define CR      0x0D

static putc_fn_t *put;

void edit_init(putc_fn_t *putc_fn)
{
    put = putc_fn;
}

/* Processes a character into an edit buffer, returns true
 * @param c the character to process
 * @param buf the edit buffer
 * @param size the size of the buffer
 * @return true if a full line was entered, false otherwise
 */
bool edit_line(char c, char *buf, int size)
{
    static int index = 0;

    switch (c) {

    case CR:
    case LF:
        // finish
        buf[index] = 0;
        put(c, 0);
        index = 0;
        return true;

    case BS:
    case 127:
        // backspace
        if (index > 0) {
            put(BS, 0);
            put(' ', 0);
            put(BS, 0);
            index--;
        } else {
            put(BELL, 0);
        }
        break;

    default:
        // try to add character to buffer
        if (index < (size - 1)) {
            buf[index++] = c;
            put(c, 0);
        } else {
            put(BELL, 0);
        }
        break;
    }
    return false;
}
