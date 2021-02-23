#include "../inc/ush.h"

static int tilde(char **str, t_ush *ush) {
    char *res = NULL;
    char *tmp = NULL;
    char **str_arr = mx_strsplit(*str, '/');

    if (strcmp(str_arr[0], "~+") == 0) {
        mx_replace_sub_str(str, 0, 1, ush->pwd);
        return 0;
    } else if (strcmp(str_arr[0], "~-") == 0) {
        mx_replace_sub_str(str, 0, 1, ush->home);
        return 0;
    } else if (strcmp(str_arr[0], "~") == 0){
        mx_replace_sub_str(str, 0, 0, ush->home);
        return 0;
    }

    if (!getpwnam((tmp = strndup(*str + 1, strlen(str_arr[0]) - 1)))) {
        fprintf(stderr, MX_ERR_PARSE_NO_SUCH_USER "%s\n", tmp);
        mx_del_strarr(&str_arr);
        free(tmp);
        return -1;
    }

    mx_replace_sub_str(str, 0, strlen(tmp), (res = mx_strjoin("/Users/", tmp)));
    mx_del_strarr(&str_arr);
    free(res);
    free(tmp);
    return 0;
}

int mx_tilde_expansion(char **argv, t_ush *ush) {
    if (!argv || !*argv) return 0;

    for (char **str_arr = argv + 1; *str_arr; str_arr++) {
        if (**str_arr != '~') continue;

        if (tilde(str_arr, ush) == -1) return -1;
    }
    return 0;
} 
