#include "../inc/libmx.h"

char *mx_strtrim(const char *str) {
    if(!str) return NULL;
    int space_before = 0;
    int space_after = 0;
    for (int i = 0; mx_isspace(str[i]); i++) {
        space_before++;
    }
    if(space_before == mx_strlen(str)) return (char *)str;
    for (int i = mx_strlen(str); mx_isspace(str[i-1]); i--) {
        space_after++;
    }
    int new_size = mx_strlen(str) - (space_before + space_after);
    return mx_strndup(&str[space_before], new_size);
}
