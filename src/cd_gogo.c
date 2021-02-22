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

static char *check_new_path(char *newpwd) {
    char **new_paths = mx_strsplit(newpwd, '/');
    char *temp = NULL;

    if (new_paths == NULL || new_paths[1] == NULL) {
        free(newpwd);
        return mx_strdup("/");
    }

    temp = mx_strjoin(temp, "/");
    for (int i = 0; new_paths[i + 1]; i++) {
        temp = mx_strjoin(temp, new_paths[i]);
        (new_paths[i + 2] != NULL) ? temp = mx_strjoin(temp, "/") : 0;
    }

    free(newpwd);
    newpwd = temp;
    mx_del_strarr(&new_paths);

    return newpwd;
}

char *gogo(char *newpwd, char **m, t_cd *in) {
    for (int i = 0; m[i]; i++) {
        if (!strcmp(m[i], "~") && i == 0) {
            continue;
        } else if (!strcmp(m[i], "..") && newpwd != NULL && strcmp(newpwd, "/") != 0) {
            newpwd = check_new_path(newpwd);
        } else if (!strcmp(m[i], ".")) {
            continue;
        } else if (strcmp(m[i], "..") != 0) {
            newpwd = new_path(newpwd, m[i], in);
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
