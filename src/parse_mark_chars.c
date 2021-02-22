#include "../inc/ush.h"

t_range *mx_is_inside_of(int i, enum e_quote type, t_frmt_lst **arr) {
    for (t_frmt_lst *p = arr[type]; p; p = p->next)
        if (i >= p->data->start && i <= p->data->end)
            return p->data;
    return NULL;
}

void mx_mark_semicolon(char *s, t_frmt_lst **arr) {
    for (t_frmt_lst *p = arr[SEMICOL]; p; p = p->next)
        if (!mx_is_inside_of(p->data->start, DOL_CMD, arr))
            s[p->data->end] = M_SEMIC;
}

void mx_mark_slash_dbl_single_quote(char *s, t_frmt_lst **arr) {
    for (t_frmt_lst *p = arr[SLASH]; p; p = p->next)
        if (!p->data->start && !mx_is_inside_of(p->data->end, DOL_CMD, arr))
            s[p->data->end] = M_SKSL;
    for (t_frmt_lst *p = arr[DBL_Q]; p; p = p->next)
        if (!mx_is_inside_of(p->data->start, DOL_CMD, arr)) {
            s[p->data->start] = M_SKP;
            s[p->data->end] = M_SKP;
        }
    for (t_frmt_lst *p = arr[SIN_Q]; p; p = p->next) {
            s[p->data->start] = M_SKP;
            s[p->data->end] = M_SKP;
    }
}

void mx_mark_chars(char *s, t_frmt_lst **arr) {
    t_range *range = NULL;

    for (int i = 0; s[i]; i++) {
        if ((range = mx_is_inside_of(i, DOL_CMD, arr))
            || (range = mx_is_inside_of(i, SIN_Q, arr))
            || (range = mx_is_inside_of(i, BCK_Q, arr)))
            i = range->end;
        else if (MX_IS_SP_TAB_NL(s[i]) && !mx_is_inside_of(i, DBL_Q, arr)
                 && (!i || s[i - 1] != M_SKSL))
            s[i] = M_DEL;
    }
    while ((s = strchr(s, M_SKSL)))
        *s = M_SKP;
}

char *mx_clear_str(char *str) {
    int new_p = 0;
    char *new = calloc(1, strlen(str) + 1);

    for (char *s = str; *s; s++) {
        if (*s == M_SKP)
            continue;
        new[new_p++] = *s;
    }
    free(str);
    return new;
}
