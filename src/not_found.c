#include "../inc/ush.h"

int mx_not_found(char *args, char *str) {
    //fg: job not found: fsh
    write(2, str, mx_strlen(str));
    write(2, " not found: ", 12);
    write(2, args, mx_strlen(args));
    write(2, "\n", 1);
    return 127;
}
