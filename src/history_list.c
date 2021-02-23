#include "../inc/ush.h"

static t_history *create_history_node(char *data) {
    t_history *history = (t_history *)malloc(sizeof(t_history) * 3);

    history->data = mx_strdup(data);
    history->next = NULL;
    history->prev = NULL;
    return history;
}

void mx_push_front_history(t_history **history, char *data) {
    t_history *tmp_history = *history;
    
    if (tmp_history == NULL) {
        *history = create_history_node(data);
    } else {
        tmp_history->prev = create_history_node(data);
        tmp_history->prev->next = tmp_history;
        tmp_history = tmp_history->prev;
        *history = tmp_history;
    } 
}

void mx_free_node(t_history *node) {
    free(node->data);
    free(node);
}

void mx_free_history(t_history **history) {
    t_history *temp_history = *history;

    while(temp_history) {
        t_history *tmp_parent = temp_history->next;

        if (temp_history->data != NULL) free(temp_history->data);
        free(temp_history);

        temp_history = NULL;
        temp_history = tmp_parent;
    }
}
