#include "../inc/ush.h"

char *mx_strpart(char *str, int index) {
    if (str == NULL) return NULL;

    char *res = mx_strnew(mx_strlen(str) - index);
    for (int i = index, k = 0; str[i]; k++, i++) res[k] = str[i];

    return res;
}
