#include "../inc/ush.h"

static t_hst *create_h(char *data) {
    t_hst *h = (t_hst *)malloc(sizeof(t_hst) * 3);

    h->data = mx_strdup(data);
    h->next = NULL;
    h->prev = NULL;
    return h;
}

void mx_push_f(t_hst **hs, char *data) {
    t_hst *p = *hs;
    
    if (p == NULL)
        *hs = create_h(data);
    else {
        p->prev = create_h(data);
        p->prev->next = p;
        p = p->prev;
        *hs = p;
    } 
}
