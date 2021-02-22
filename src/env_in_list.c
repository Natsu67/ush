#include "../inc/ush.h"

static bool change_var(t_list **env_set, char *src) {
    t_list *tmp = *env_set;
    char **t_set = NULL;
    char **t_var = NULL;

    if ((mx_get_char_index(src, '=') < 0))
        return true;
    t_var = mx_strsplit(src, '=');
    for( ; tmp; tmp = tmp->next) { //заменить
        if (mx_get_substr_index(tmp->data, t_var[0]) >= 0) {
            t_set = mx_strsplit(tmp->data, '=');
            if (mx_strcmp_null(t_set[0], t_var[0]) == 0) { //t_var[1] != NULL
                free(tmp->data);
                tmp->data = mx_strdup(src);
                mx_del_strarr(&t_set);
                mx_del_strarr(&t_var);
                return true;
            }
            mx_del_strarr(&t_set);
        }
    }
    mx_del_strarr(&t_var);
    return false;
}

static bool f_case(char ***v) {
    char **var = *v;
    write(2, "export: not valid in this context: ", 
        mx_strlen("export: not valid in this context: "));
    write(2, var[0], mx_strlen(var[0]));
    write(2, "\n", 1);
    mx_del_strarr(v);
    return false;
}

static bool s_case(char ***v) {
    char **var = *v;
    write(2, "export: not an identifier: ", 
        mx_strlen("export: not an identifier: "));
    write(2, var[0], mx_strlen(var[0]));
    write(2, "\n", 1);
    mx_del_strarr(v);
    return false;
}

static bool var_check(char *src) {
    char **var = mx_strsplit(src, '=');

    if((var == NULL || var[0] == NULL) || src[0] == '=') {
        write(1, "u$h: bad assignment\n", 20);
        return false;
    }
    if (mx_isalpha(var[0][0])) {
        for (int i = 1; var[0][i]; i++) {
            if (var[0][i] < 48 && var[0][i] > 122)
                return f_case(&var);
        }
    }
    else if (mx_isdigit(var[0][0]))
        return s_case(&var);
    else 
        return f_case(&var);
    mx_del_strarr(&var);
    return true;
}

bool mx_env_in_list(t_list **env_set, char *src) {
    t_list *tmp = *env_set;

    if (var_check(src)) {
        if (tmp->data == NULL) { //если пусто
            tmp->data = mx_strjoin(tmp->data, src);
        }
        else if (!change_var(env_set, src)) //добавить
            mx_push_backdup(env_set, src);
        return true;
    }
    return false;
}
