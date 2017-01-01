#include <stdio.h>

typedef int (putc_fn_t)(char c, FILE *file);

void edit_init(putc_fn_t *putc);
bool edit_line(char c, char *buf, int size);


