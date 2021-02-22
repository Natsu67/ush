#include "../inc/ush.h"

char *mx_strpart(char *str, int index) {
    char *res = NULL;

    if (str == NULL)
        return NULL;
    res = mx_strnew(strlen(str) - index);
    for (int i = index, k = 0; str[i]; k++, i++)
        res[k] = str[i];
    return res;
}
