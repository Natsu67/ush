#include "../inc/ush.h"

static void paste_subst(char **str, char *replace, t_range *rep_range,
                            t_frmt_lst **arr) {
    int shift = strlen(replace) - (rep_range->end - rep_range->start + 1);

    for (t_frmt_lst *p = arr[OUT_SUB]->next; p; p = p->next) {
        p->data->start += shift;
        p->data->end += shift;
    }
    for (t_frmt_lst *dblq_p = arr[OUT_DBQ]; dblq_p; dblq_p = dblq_p->next) {
        if (dblq_p->data->start > rep_range->start) {
            dblq_p->data->start += shift;
            dblq_p->data->end += shift;
        }
        else if (dblq_p->data->end > rep_range->end)
            dblq_p->data->end += shift;
    }
    mx_replace_sub_str(str, rep_range->start, rep_range->end, replace);
    free(replace);
}

static char *mark_sbst_output(char *str, bool in_quotes) {
    char *s;

    if (!str || !*str)
        return str;
    for (s = str + strlen(str) - 1; *s == '\n'; s--)
        *s = M_SKP;
    if (in_quotes)
        return str;

    for (s = str; *s && *s != M_SKP; s++) {
        if (MX_IS_SP_TAB_NL(*s))
            *s = M_DEL;
    }
    return str;
}

int mx_handle_substitutions(char **str, t_frmt_lst **arr, t_ush *ush) {
    char *replace = NULL;
    char *process_out = NULL;

    mx_create_outer_subst_n_dblq_list(*str, arr);
    for (t_frmt_lst *lst; (lst = arr[OUT_SUB]); mx_pop_format(arr + OUT_SUB)) {
        replace = mx_get_subst_replace_str(str, lst, ush);
        if ((*str)[lst->data->start] == '`'
            || (*str)[lst->data->start + 1] == '(') {
            process_out = mx_get_subst_outputs(replace, ush);
            if (!process_out)
                return -1;
            free(replace);
            replace = mark_sbst_output(process_out,
                mx_is_inside_of(lst->data->start, OUT_DBQ, arr) ? 1 : 0);
        }
        paste_subst(str, replace, lst->data, arr);  // replace freed
    }
    return 0;
}
