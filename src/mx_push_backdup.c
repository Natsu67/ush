#include "../inc/ush.h"

void mx_push_backdup(t_list **list, void *data) {
    if (!list) return;

    t_list *tmp = *list;
    if (tmp) {
        while (tmp->next) tmp = tmp->next;
        tmp->next = mx_create_node(mx_strdup(data));
    }
    else *list = mx_create_node(mx_strdup(data));
}
