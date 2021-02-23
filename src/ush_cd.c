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

char *mx_create_newpwd(char *newpwd, char **m, t_cd *in) {
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

static int false_cd(char *name, t_cd *num) {
    if (num->error == 1) mx_printerr("cd: no such file or directory: ");
    else if (num->error  == 2) mx_printerr("cd: not a directory: ");
    else if (num->error == 4) mx_printerr("cd: permission denied: ");
    mx_printerr(name);
    mx_printerr("\n");
    free(num->newpwd);
    free(num);
    errno = 0;
    return 1;
}

static int env_work(t_ush *ush, char *pwd, char *args, t_cd *in) {
    if (!mx_opencheck(in->newpwd, &in->error, in->flag_s)) {
        free(pwd);
        return false_cd(in->newpwd, in);
    }
    if (chdir(in->newpwd) == -1) {
        free(pwd);
        in->error = 1;
        return false_cd(args, in);
    }
    if (in->flag_P == 1) {
        free(in->newpwd);
        in->newpwd = NULL;
        in->newpwd = getcwd(NULL, 0);
        setenv("PWD", in->newpwd, 1);
    } else {
        setenv("PWD", in->newpwd, 1);
    }
    setenv("OLDPWD", pwd, 1);
    free(pwd);
    free(ush->pwd);
    ush->pwd = mx_strdup(in->newpwd);
    free(in->newpwd);
    free(in);
    return 2;
}

static char *check_slash(char **args, int f, char *pwd) {
    char *newpwd = NULL;

    if (args[f][0] != '/')  newpwd = mx_strdup(pwd); 

    return newpwd;
}

static int check_args(char **args, char *pwd, t_cd *in, t_ush *ush) {
    if (args[1] == NULL) {
        in->newpwd = mx_strdup(ush->home);
        return env_work(ush, pwd, args[0], in);
    }
    if (!strcmp(args[in->flag_num], "-") || !strcmp(args[in->flag_num], "~-")) {
        in->newpwd  = mx_strdup(getenv("OLDPWD"));
        return env_work(ush, pwd, args[in->flag_num], in);
    }
    if (!strcmp(args[in->flag_num], "~+")) {
        in->newpwd = mx_strdup(getenv("PWD"));
        return env_work(ush, pwd, args[in->flag_num], in);
    }
    if (strcmp(args[in->flag_num], "/") == 0) {
        in->newpwd = mx_strdup("/");
        return env_work(ush, pwd, args[in->flag_num], in);
    }
    return 0;
}

int mx_ush_cd(char **args, t_ush *ush) {
    char *pwd = mx_strdup(ush->pwd);
    char **splitted_path = NULL;
    t_cd *in = (t_cd *)malloc(sizeof(t_cd) * 5);
    
    in->flag_num = 1;
    if (args[1] && (!strcmp(args[1], "-s") || !strcmp(args[1], "-P"))){
        in->flag_num = 2;
        !strcmp(args[1], "-s") ? in->flag_s = 1 : 0;
        !strcmp(args[1], "-P") ? in->flag_P = 1 : 0;
    }

    if (check_args(args, pwd, in, ush) > 0) return 0;

    in->newpwd = check_slash(args, in->flag_num, pwd);
    splitted_path = mx_strsplit(args[in->flag_num], '/');
    in->newpwd = mx_create_newpwd(in->newpwd, splitted_path, in);
    if (in->newpwd == NULL) {
        free(pwd);
        return false_cd(args[in->flag_num], in);
    }
    env_work(ush ,pwd, args[in->flag_num], in);
    return 0;
}
