#include "../inc/ush.h"

static char *param_dollar_expansion(char **str, t_frmt_lst *list, t_ush *ush) {
    char *replace = NULL;
    char *buf = NULL;

    if ((*str)[list->data->start + 1] == '?')
        replace = mx_itoa(ush->last_return);
    else {
        buf = strndup(*str + list->data->start + 1,
                      list->data->end - list->data->start);
        replace = (replace = getenv(buf)) ? strdup(replace)
                                          : calloc(1, 1);
        free(buf);
    }
    return replace;
}

static char *param_dollar_braces_expansion(char **str, t_frmt_lst *list,
                                           t_ush *ush) {
    char *replace = NULL;
    char *buf = NULL;

    if ((*str)[list->data->start + 2] == '?')
        replace = mx_itoa(ush->last_return);
    else {
        buf = strndup(*str + list->data->start + 2,
                      list->data->end - list->data->start - 2);
        replace = (replace = getenv(buf)) ? strdup(replace)
                                          : calloc(1, 1);
        free(buf);
    }
    return replace;
}

static void unslash_str(char **str) {
    int new_p = 0;
    char *new = calloc(1, strlen(*str) + 1);
    bool flag = 0;

    for (char *s = *str; *s; s++) {
        if (*s == '\\') {
            if (!flag && mx_get_char_index("`$\"\\", *(s + 1)) >= 0) {
                flag = 1;
                continue;
            }
            flag = 0;
        }
        else if (flag)
            flag = 0;
        new[new_p++] = *s;
    }
    free(*str);
    *str = new;
}

char *mx_get_subst_replace_str(char **str, t_frmt_lst *list, t_ush *ush) {
    char *replace = NULL;

    if (!str || !list)
        return NULL;
    if ((*str)[list->data->start] == '`') {
        replace = strndup(*str + list->data->start + 1,
                          list->data->end - list->data->start - 1);
        unslash_str(&replace);
    }
    else if ((*str)[list->data->start + 1] == '(')
        replace = strndup(*str + list->data->start + 2,
                          list->data->end - list->data->start - 2);
    else if ((*str)[list->data->start + 1] == '{')
        replace = param_dollar_braces_expansion(str, list, ush);
    else
        replace = param_dollar_expansion(str, list, ush);
    if (!replace)
        replace = calloc(1, 1);
    return replace;
}
