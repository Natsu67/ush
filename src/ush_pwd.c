#include "../inc/ush.h"

int mx_ush_pwd(char **args, t_ush *ush) {
    char *pwd = getcwd(NULL, 0);
    char **m = NULL;

    if (args[1] != NULL && !strcmp(args[1], "-P")) {
        write(1, pwd, strlen(pwd));
        write(1, "\n", 1);
        free(pwd);
        return 0;
    }
    m = mx_strsplit(ush->pwd, '/');
    if (m[0] == NULL)
        mx_printstr("/");
    else for (int i = 0; m[i]; i++){
        mx_printstr("/");
        mx_printstr(m[i]);
    }
    write(1, "\n", 1);
    mx_del_strarr(&m);
    free(pwd);
    return 0;
}
