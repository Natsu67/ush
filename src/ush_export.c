#include "../inc/ush.h"

static char *copy_targ(int count, char *str) {
    int start = count + 1;
    int end = count;
    char *res;

    while (str[start - 1] != '=')
        start++;
    while (str[end] && str[end] != ';')
        end++;
    res = mx_strnew(end - start);
    for (int i = start, tm = 0; i < end; i++, tm++)
        res[tm] = str[i];
    return res;
}

static char *zad_pered(char *args, t_list *l) {
    int res;
    char *r = NULL;
    char *tmp = mx_strjoin(args, "=");

    for (t_list *list = l; list; list = list->next){
        res = mx_get_substr_index(list->data, tmp);
        if (res >= 0) {
            r = copy_targ(res, list->data);
            mx_strdel(&tmp);
            return r;
        }
    }
    r = mx_strdup("\n");
    mx_strdel(&tmp);
    return r;
}

static char *check(char *args, t_list *start_h) {
    t_list *l = start_h;
    char *res = NULL;

    if (mx_get_char_index(args, '=') > 0)
        return res;
    else
        res = zad_pered(args, l);
    return res;
}

static void ch_env (char *a, char *b) {
    char **tmp;
    char *c = NULL;
    char *d = NULL;

    c = mx_strjoin(c, b);
    tmp = mx_strsplit(a, '=');
    if (tmp[1] != NULL && c == NULL)
        c = mx_strjoin(c, tmp[1]);
    d = mx_strdup(tmp[0]);
    mx_del_strarr(&tmp);
    if (c == NULL)
        c = mx_strdup("\0");
    setenv(d, c, 1);
    mx_strdel(&c);
    mx_strdel(&d);
}

int mx_ush_export(char **args, t_list **env_set) {
    char *var = NULL;

    if (args[1] == NULL)
        return mx_env_print();
     // дополнние сета експортом
    for (int i = 1; args[i]; i++) {
        if (mx_env_in_list(env_set, args[i])) {
            var = check(args[i], *env_set);
            if (mx_strcmp_null(var, "\n") != 0)
                ch_env (args[i], var);
            if (var != NULL)
                mx_strdel(&var);
        }
    }
    return 0;
}
