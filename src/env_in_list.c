#include "../inc/ush.h"

static bool change_var(t_list **env_set, char *src) {
    t_list *temp = *env_set;
    char **t_set = NULL;
    char **t_var = NULL;

    if ((mx_get_char_index(src, '=') < 0)) return true;

    t_var = mx_strsplit(src, '=');
    for( ; temp; temp = temp->next) {
        if (mx_get_substr_index(temp->data, t_var[0]) >= 0) {
            t_set = mx_strsplit(temp->data, '=');
            
            if (mx_strcmp(t_set[0], t_var[0]) == 0) { //t_var[1] != NULL
                free(temp->data);
                temp->data = mx_strdup(src);
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

static bool f_case(char ***var) {
    char **temp_var = *var;

    mx_printerr("export: not valid in this context: ");
    mx_printerr(temp_var[0]);
    mx_printerr("\n");

    mx_del_strarr(var);
    return false;
}

static bool s_case(char ***var) {
    char **temp_var = *var;

    mx_printerr("export: not an identifier: ");
    mx_printerr(temp_var[0]);
    mx_printerr("\n");

    mx_del_strarr(var);
    return false;
}

static bool var_check(char *src) {
    char **temp_var = mx_strsplit(src, '=');

    if((temp_var == NULL || temp_var[0] == NULL) || src[0] == '=') {
        mx_printerr("ush: bad assignment\n");
        return false;
    }

    if (mx_isalpha(temp_var[0][0])) {
        for (int i = 1; temp_var[0][i]; i++) {
            if (temp_var[0][i] < 48 && temp_var[0][i] > 122)
                return f_case(&temp_var);
        }
    }
    else if (mx_isdigit(temp_var[0][0])) return s_case(&temp_var);
    else return f_case(&temp_var);

    mx_del_strarr(&temp_var);
    return true;
}

bool mx_env_in_list(t_list **env_set, char *src) {
    t_list *temp = *env_set;

    if (var_check(src)) {
        if (temp->data == NULL) temp->data = mx_strjoin(temp->data, src);
        else if (!change_var(env_set, src)) mx_push_back_list(env_set, src);

        return true;
    }

    return false;
}
