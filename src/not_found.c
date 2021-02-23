#include "../inc/ush.h"

int mx_not_found(char *args, char *str) {
    mx_printerror(str);
    mx_printerror(" not found: ");
    mx_printerror(args);
    mx_printerror("\n");
    return 0;
}

int mx_no_such(char *name, char *args, char *str) {
    write(2, name, mx_strlen(name));
    write(2, ": ", 2);
    write(2, args, mx_strlen(args)); 
    write(2, ": no such  ", 11);
    write(2, str, mx_strlen(str));
    write(2, "\n", 1);
    return 1;
}
