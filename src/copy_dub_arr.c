#include "../inc/ush.h"

char **mx_copy_dub_arr(char **args) {
    int count = 0;
    char **res = NULL;

    if(args == NULL)
        return res;
    for (; args[count]; count++);
    res = (char **) malloc(sizeof(char *) * (count + 1));
    for (int i = 0; i <= count; i++)
        res[i] = NULL;
    for (int i = 0; i < count; i++)
        res[i]= mx_strjoin(res[i], args[i]);
    return res; 
}
