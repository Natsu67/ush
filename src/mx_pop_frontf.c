#include "../inc/ush.h"

void mx_pop_frontf(t_list **head) {
    if (head == NULL || *head == NULL) return;

    t_list *cpy_list, *tmp;

    cpy_list = *head;
    tmp = cpy_list->next;
    free(cpy_list->data);
    cpy_list->data = NULL;
    free(*head);
    
    *head = NULL;
    *head = tmp;
}
