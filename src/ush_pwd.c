#include "../inc/ush.h"

int mx_ush_pwd(char **args, t_ush *ush) {
    char *pwd = getcwd(NULL, 0);
    char **splitted_path = NULL;

    if (args[1] != NULL && !strcmp(args[1], "-P")) {
        mx_printstr(pwd);
        mx_printstr("\n");
        free(pwd);
        return 0;
    } else if(args[1] != NULL) {
        write(2, "pwd: too many arguments\n", 24);
        return 0;
    }
    splitted_path = mx_strsplit(ush->pwd, '/');
    if (splitted_path[0] == NULL) mx_printstr("/");
    else for (int i = 0; splitted_path[i]; i++){
        mx_printstr("/");
        mx_printstr(splitted_path[i]);
    }
    mx_printstr("\n");
    mx_del_strarr(&m);
    free(pwd);
    return 0;
}
