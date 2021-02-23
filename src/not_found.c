#include "../inc/ush.h"

int mx_not_found(char *args, char *str) {
    mx_printerr(str);
    mx_printerr(" not found: ");
    mx_printerr(args);
    mx_printerr("\n");
    return 0;
}

int mx_no_such(char *name, char *args, char *str) {
    mx_printerr(name);
    mx_printerr(": ");
    mx_printerr(args); 
    mx_printerr(": no such  ");
    mx_printerr(str);
    mx_printerr("\n");
    return 1;
}
