#include "../inc/ush.h"

static bool check(char **args) {
    extern char **environ;
    char **tmp = NULL;

    if (mx_get_substr_index(args[0], "/") > 0)
        return true;
    else {
        for (int i = 0; environ[i]; i++) {
            if (mx_get_substr_index(environ[i], "PATH") >= 0) {
                tmp = mx_strsplit(environ[i], '=');
                if (mx_strcmp_null(tmp[0], "PATH") == 0) {
                mx_del_strarr(&tmp);
                return true;
                }
                mx_del_strarr(&tmp);
            }
        }
    }
    return false;
}

int builtin(char **args, t_ush *ush) {
    if (!strcmp(args[0], "echo"))
        return mx_ush_echo(args);
    if (!strcmp(args[0], "cd") || !strcmp(args[0], "chdir"))
        return mx_ush_cd(args, ush);
    if (!strcmp(args[0], "pwd"))
        return mx_ush_pwd(args, ush);
    if (!strcmp(args[0], "env"))
        return mx_ush_env(args, &ush->jobs);
    if (!strcmp(args[0], "exit"))
        return mx_ush_exit(args, ush);
    if (!strcmp(args[0], "which"))
        return mx_ush_which(args);
    if (!strcmp(args[0], "export"))
        return mx_ush_export(args, &ush->env_set);
    if (!strcmp(args[0], "unset"))
        return mx_ush_unset(args, &ush->env_set);
    if (!strcmp(args[0], "fg"))
        return mx_ush_fg(args, &ush->jobs);
    if (!strcmp(args[0], "jobs"))
        return mx_ush_jobs(args, &ush->jobs);
    if (!strcmp(args[0], "true"))
        return 0;
    if (!strcmp(args[0], "false"))
        return 1;
    return -1;
}

int mx_detect_builds(char **args, t_ush *ush) {
    int bins = 1;

    if (!args || !args[0])
        return 0;
    if ((bins = builtin(args, ush)) != -1) 
        return bins;
    bins = mx_detect_exp(args, ush->hist, &ush->env_set);;
    if (bins != 3) 
        return bins;
    else if (check(args))
        return mx_straus_proc(args, &ush->jobs);
    return mx_not_found(args[0], "ush: command"); //error
}
