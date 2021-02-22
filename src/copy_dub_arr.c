#include "../inc/ush.h"

char **mx_copy_dub_arr(char **args) {
    int size = 0;
    char **res = NULL;

    if(args == NULL) return res;
    
    for (; args[size]; size++);
    res = (char **)malloc((size + 1) * sizeof(char *));

    for (int i = 0; i <= size; i++) res[i] = NULL;

    for (int i = 0; i < size; i++) res[i] = mx_strjoin(res[i], args[i]);

    return res; 
}
