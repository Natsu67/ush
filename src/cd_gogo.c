#include "../inc/ush.h"

static char *new_path(char *newpwd, char *splitted_path, t_cd *in) {
    struct stat inf;

    lstat(newpwd, &inf);

    newpwd = mx_strjoin(newpwd, "/");
    newpwd = mx_strjoin(newpwd, splitted_path);
    
    if (!mx_opencheck(newpwd, &in->error, in->flag_s)) {
        free(newpwd);
        return NULL;
    }
    return newpwd;
}

static char *dotdot(char *newpwd) {
    char **p = mx_strsplit(newpwd, '/');
    char *tmp = NULL;

    if (p == NULL || p[1] == NULL) {
        free(newpwd);
        return mx_strdup("/");
    }
    tmp = mx_strjoin(tmp, "/");
    for (int i = 0; p[i + 1]; i++) {
        tmp = mx_strjoin(tmp, p[i]);
        (p[i + 2] != NULL) ? tmp = mx_strjoin(tmp, "/") : 0;
    }
    free(newpwd);
    newpwd = tmp;
    mx_del_strarr(&p);
    return newpwd;
}

char *gogo(char *newpwd, char **m, t_cd *in) {
    for (int j = 0; m[j]; j++) {
        if (!strcmp(m[j], "~") && j == 0) {
            continue;
        } else if (!strcmp(m[j], "..") && newpwd != NULL && strcmp(newpwd, "/") != 0) {
            newpwd = dotdot(newpwd);
        } else if (!strcmp(m[j], ".")) {
            continue;
        } else if (strcmp(m[j], "..") != 0) {
            newpwd = new_path(newpwd, m[j], in);
            if (in->error > 0) {
                mx_del_strarr(&m);
                errno = 0;
                return newpwd;
            }
        }
    }
    mx_del_strarr(&m);
    return newpwd;
}
