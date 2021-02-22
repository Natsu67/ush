#include "../inc/ush.h"

static char **env_copy(char **environ) {
    char **res = NULL;
    int len = 0;

    while(environ[len])
        len++;
    res = (char **)malloc(sizeof(char *) * (len + 1));
    res[len] = NULL;
    for (int i = 0; environ[i]; i++) 
        res[i] = mx_strdup(environ[i]);
    return res;
}

static int usage(int f, char *arg) {
    if (f == 1)
        write(2, "usage: env [-iuP] [name=value ...] "
                "[utility [argument ...]]\n", 60);
    else {
        write(2, "env: '", 6);
        write(2, arg, mx_strlen(arg));
        write(2, "': No such file or directory\n", 29);
    }
    return 1;
}

static void e_work(char ***environ, t_env *env) {
    char **m = NULL;

    if (env->flag_i != 1) {
        *environ = env->clear;
        env->clear[0] = NULL;
    }
    for (int i = 0; env->clear[i]; i++)
        env->clear[i] = NULL;
    for (int i = 0; env->n[i]; i++) {
        m = mx_strsplit(env->n[i], '=');
        if (m[1])
            setenv(m[0], m[1], 1);
        else 
            setenv(m[0], "\0", 1);
        mx_del_strarr(&m);
    }
    mx_del_strarr(&env->n);
    free(env);
}

static int check_args(char ***args, t_env *env, char ***environ) {
    env->n = env_copy(*environ);
    if ((*args)++ && mx_get_char_index(**args, '-') == 0) {
        if (mx_get_char_index(**args, 'i') > 0) {
            env->flag_i = 1;
            *environ = env->clear;
            env->clear[0] = NULL;
            (*args)++;
        }
        else if (mx_get_char_index(**args, 'u') > 0) {
            env->flag_u = 1;
            (*args)++;
        }
        else if (mx_get_char_index(**args, 'P') > 0) {
            (*args)++;
            setenv("PATH", *(*args)++, 1);
        }
        else if (mx_get_char_index(**args, '-') != 1)
            return usage(1, NULL);
    }
    return 0;
}

int mx_ush_env(char **args, t_jobs **jobs) {
    extern char **environ;
    t_env *env = (t_env *)malloc(7 * sizeof(t_env));
    int status = 0;

    if (check_args(&args, env, &environ) > 0) {
        mx_del_strarr(&env->n);
        free(env);
        return 1;
    }
    if (env->flag_i != 1 && env->flag_u == 1) {
        unsetenv(*args);
        args++;
    }
    else for ( ; *args && mx_get_char_index(*args, '=') > -1; args++)
        putenv(*args);
    if (*args)
        status = mx_straus_proc(args, jobs);
    else for (int i = 0; environ[i]; i++)
        printf("%s\n", environ[i]);
    e_work(&environ, env);
    if (status == -1)
        return usage(2, *args);
    return status;
}
