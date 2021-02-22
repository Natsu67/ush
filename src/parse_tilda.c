#include "../inc/ush.h"

static void tilde2(char **str, char ***arr, t_ush *ush) {
    if (strcmp((*arr)[0], "~+") == 0) {
        mx_replace_sub_str(str, 0, 1, ush->pwd);
    }
    else if (strcmp((*arr)[0], "~-") == 0) {
        mx_replace_sub_str(str, 0, 1, ush->home);
    }
    else
        mx_replace_sub_str(str, 0, 0, ush->home);
    mx_del_strarr(arr);
}

static void no_such_user(char *tmp, char ***m) {
    fprintf(stderr, MX_ERR_PARSE_NO_SUCH_USER "%s\n", tmp);
    mx_del_strarr(m);
    free(tmp);
}

static int tilde(char **str, t_ush *ush) {
    char *res = NULL;
    char *tmp = NULL;
    char **m = mx_strsplit(*str, '/');

    if (!strcmp(m[0], "~+") || !strcmp(m[0], "~-") || !strcmp(m[0], "~")) {
        tilde2(str, &m, ush);
        return 0;
    }
    if (!getpwnam((tmp = strndup(*str + 1, strlen(m[0]) - 1)))) {
        no_such_user(tmp, &m);
        return -1;
    }
    mx_replace_sub_str(str, 0, strlen(tmp),
        (res = mx_strjoin("/Users/", tmp)));
    mx_del_strarr(&m);
    free(res);
    free(tmp);
    return 0;
}

int mx_tilde_expansion(char **argv, t_ush *ush) {
    if (!argv || !*argv)
        return 0;
    for (char **s = argv + 1; *s; s++) {
        if (**s != '~')
            continue;
        if (tilde(s, ush) == -1) {
            return -1;
        }
    }
    return 0;
} 
