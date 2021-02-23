#include "../inc/ush.h"

void mx_free_list(t_hst **list) {
    t_hst *temp = *list;

    while(temp) {
        t_hst *hren = temp->next;

        if (temp->data != NULL) free(temp->data);
        free(temp);

        temp = NULL;
        temp = hren;
    }
}
