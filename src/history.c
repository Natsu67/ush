#include "../inc/ush.h"

static t_hst *create_hst_node(char *data) {
    t_hst *hst = (t_hst *)malloc(sizeof(t_hst) * 3);

    hst->data = mx_strdup(data);
    hst->next = NULL;
    hst->prev = NULL;
    return hst;
}

void mx_push_f(t_hst **hst, char *data) {
    t_hst *tmp_hst = *hst;
    
    if (tmp_hst == NULL){
        *hst = create_hst_node(data);
    } else {
        tmp_hst->prev = create_hst_node(data);
        tmp_hst->prev->next = tmp_hst;
        tmp_hst = tmp_hst->prev;
        *hst = tmp_hst;
    } 
}
