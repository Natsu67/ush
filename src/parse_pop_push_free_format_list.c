#include "../inc/ush.h"

void mx_pop_format(t_frmt_lst **del) {
    if (!del || !*del)
        return;
    free((*del)->data);
    mx_pop_front((t_list **)del);
}

void mx_push_format(t_frmt_lst **add, int start, int end, t_frmt_lst **del) {
    mx_push_front((t_list **)add, malloc(sizeof(t_range)));
    (*add)->data->start = start;
    (*add)->data->end = end;
    if (del)
        mx_pop_format(del);
}

void mx_push_back_format(t_frmt_lst **add, int start, int end,
                         t_frmt_lst **del) {
    t_range *new = malloc(sizeof(t_range));

    new->start = start;
    new->end = end;
    mx_push_back((t_list **)add, new);
    if (del)
        mx_pop_format(del);
}

void mx_free_format_lists(t_frmt_lst **arr) {
    if (!arr)
        return;
    for (int i = 0; i < NUM_Q; i++)
        while (arr[i])
            mx_pop_format(arr + i);
}
