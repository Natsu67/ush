#include "../inc/ush.h"

void mx_free_list2(t_list **list) {
    t_list *tmp = *list;
    while(tmp) {
        t_list *hren = tmp->next;
        if (tmp->data != NULL) 
            free(tmp->data);
        free(tmp);
        tmp = NULL;
        tmp = hren;
    }
}
