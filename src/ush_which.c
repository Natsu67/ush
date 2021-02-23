#include "../inc/ush.h"

static int check_access(char *argument, int flag) {
    int is_access = 1;
    char *temp = NULL;

    char **splitted_path = mx_strsplit(getenv("PATH"), ':');
    for (int i = 0; splitted_path[i]; i++) {
        temp = mx_strjoin(splitted_path[i], "/");
        temp = mx_strjoin(temp, argument);
        if (access(temp, F_OK) == 0) {
            flag != 2 ? printf("%s\n", temp) : 0;
            is_access = 0;
            if (flag == 0) break;
        }
        mx_strdel(&temp);
    }
    mx_strdel(&temp);
    mx_del_strarr(&splitted_path);
    return is_access;
}

static int check_if_builtin(char *argument) {
    char *builtin[] = {"which", "unset", "cd", "chdir", "echo", "pwd", "exit", "alias", "env", "kill", "false", "true", "bg", "fg", "if", "jobs", "export", "exec", "continue", "break", NULL};

    for (int i = 0; builtin[i]; i++) {
        if (strcmp(builtin[i], argument) == 0) {
            printf("%s: ush built-in command\n", argument);
            return 0;
        }
    }
    return 1;
}

int mx_ush_which(char **args) {
    int flag = 0;

    if (args[1] == NULL) return 1;

    if (strcmp(args[1], "-a") == 0) {
        flag = 1;
        args++;
    } else if (strcmp(args[1], "-s") == 0) {
        flag = 2;
        args++;
    }
    args++;

    int is_access = 0;
    
    while(*args) {
        
        is_access = 0;
        if (check_if_builtin(*args)) {
            is_access = check_access(*args, flag);
            if (is_access == 1 && flag != 2) {
                mx_printstr(*args);
                write(2, " not found\n", 11);
            }
        }
        args++;
    }
    return 1;
}
