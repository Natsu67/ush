#include "../inc/ush.h"

char *mx_cut_str_forjob(char *args) {
    char *res = NULL;

    if (args == NULL || args[0] == '\0') //%
        return res;
    else if (args[0] != '%' || (args[0] == '/' && args[0] == '\0')) // fg alpha
        res = mx_strjoin(res, args);
    else {
        res = mx_strjoin(res, &args[1]);
    }
    return res;
}
