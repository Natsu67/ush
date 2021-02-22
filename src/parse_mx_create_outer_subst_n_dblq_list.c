#include "../inc/ush.h"

static bool is_outer(int start, t_frmt_lst **arr) {
    t_frmt_lst *p;

    for (p = arr[DOL_CMD]; p; p = p->next)
        if (start > p->data->start && start < p->data->end)
            return false;
    return true;
}

static t_range *is_subst(t_frmt_lst **arr, int start, enum e_quote *type) {
    enum e_quote subs[4] = {BCK_Q, DOL_CMD, DOL_P, DOL_BP};

    for (int i = 0; i < 4; i++) {
        for (t_frmt_lst *p = arr[subs[i]]; p; p = p->next)
            if (p->data->start == start) {
                *type = subs[i];
                return p->data;
            }        
    }
    return NULL;
}

static t_range *is_dble_q(t_frmt_lst **arr, int start) {
    for (t_frmt_lst *p = arr[DBL_Q]; p; p = p->next)
        if (start == p->data->start)
            return p->data;
    return NULL;
}

void mx_create_outer_subst_n_dblq_list(char *s, t_frmt_lst **arr) {
    enum e_quote type = 0;
    t_range *range = NULL;

    for (int i = 0; s[i]; i++) {
        if ((s[i] == '$' || s[i] == '`') && (range = is_subst(arr, i, &type))
            && is_outer(i, arr)) {
            mx_push_back_format(arr + OUT_SUB, range->start, range->end, NULL);
            i = range->end;
        }
        else if (s[i] == M_SKP && (range = is_dble_q(arr, i))
                 && is_outer(i, arr)) {
            mx_push_back_format(arr + OUT_DBQ, range->start, range->end, NULL);
        }
    }
}
