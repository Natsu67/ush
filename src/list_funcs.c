#include "../inc/ush.h"

void mx_push_back_list(t_list **list, void *data) {
    if (!list) return;

    t_list *tmp = *list;
    if (tmp) {
        while (tmp->next) tmp = tmp->next;
        tmp->next = mx_create_node(mx_strdup(data));
    }
    else *list = mx_create_node(mx_strdup(data));
}

void mx_pop_front_list(t_list **head) {
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

void mx_free_list(t_list **list) {
    t_list *temp_list = *list;

    while(temp_list) {
        t_list *tmp_parent = temp_list->next;

        if (temp_list->data != NULL) free(temp_list->data);
        free(temp_list);

        temp_list = NULL;
        temp_list = tmp_parent;
    }
}

