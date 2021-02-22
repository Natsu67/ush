#include "../inc/ush.h"

void mx_pop_frontf(t_list **head) {
    t_list *p;
    t_list *t;

    if (head == NULL || *head == NULL)
        return;
    p = *head;
    t = p->next;
    free(p->data);
    p->data = NULL;
    free(*head);
    *head = NULL;
    *head = t;
}
