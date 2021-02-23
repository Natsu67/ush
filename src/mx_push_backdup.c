#include "../inc/ush.h"

void mx_push_backdup(t_list **list, void *data) {
    if (!list) return;

    t_list *p = *list;
    if (p) {
        while (p->next) p = p->next;
        p->next = mx_create_node(mx_strdup(data));
    }
    else *list = mx_create_node(mx_strdup(data));
}
