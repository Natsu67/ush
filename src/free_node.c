#include "../inc/ush.h"

void mx_free_node(t_hst *node) {
    free(node->data);
    free(node);
}
