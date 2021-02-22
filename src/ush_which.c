#include "../inc/ush.h"

static int check_access(char *args, int flag) {
    char *tmp = NULL;
    int f = 1;
    char *path = getenv("PATH");
    char **m = NULL;

    m = mx_strsplit(path, ':');
    for (int i = 0; m[i]; i++) {
        tmp = mx_strjoin(m[i], "/");
        tmp = mx_delit_fre(tmp, args);
        if (access(tmp, F_OK) == 0) {
            flag != 2 ? printf("%s\n", tmp) : 0;
            f = 0;
            if (flag == 0)
                break ;
        }
        mx_strdel(&tmp);
    }
    mx_strdel(&tmp);
    mx_del_strarr(&m);
    return f;
}

static int built_check(char *args, int flag) {
    char *built[] = {"cd", "pwd", "exit", "which", "env", "fg", 
        "jobs", "export", "unset", "false", "true", 
            "echo", "chdir", NULL};
    
    for (int i = 0; built[i]; i++) {
        if (!strcmp(built[i], args)) {
            printf("%s: ush built-in command\n", built[i]);
            return flag;
        }
    }
    return 1;
}

int mx_ush_which(char **args) {
    int f = 1;
    int flag = 0;
    
    if (args[1] == NULL)
        return 1;
    if (args[1][0] == '-' && (args[1][1] == 'a' || args[1][1] == 's')){
        args[1][1] == 'a' ? flag = 1 : 0;
        args[1][1] == 's' ? flag = 2 : 0;
        args++;
    }
    for (args++; *args; args++) {
        f = 0;
        if (built_check(*args, flag)) {
            f = check_access(*args, flag);
            if (f == 1 && flag != 2) {
                mx_printstr(*args);
                write(2, " not found\n", 11);
            }
        }
    }
    return f;
}
