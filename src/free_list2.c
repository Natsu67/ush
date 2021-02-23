#include "../inc/ush.h"

void mx_free_list2(t_list **list) {
    t_list *temp = *list;

    while(temp) {
        t_list *hren = temp->next;

        if (temp->data != NULL) free(temp->data);
        free(temp);

        temp = NULL;
        temp = hren;
    }
}
