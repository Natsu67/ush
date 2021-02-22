#include "../inc/ush.h"

static char *hist_but(char *line, char *data, int *x) {
    for (; *x > 0; (*x)--) {
        printf("%c[1C", 27);
        fflush(stdout);
    }
    for (int i = 0; line != NULL && i < mx_strlen(line); i++)
        write(1, "\b \b", 3);
    mx_strdel(&line);
    write(1, data, mx_strlen(data));
    line = mx_strdup(data);
    return line;
}

static char *button(t_hst **hs, char *line, int buf, int *x) {
    t_hst *h = *hs;

    if (buf == 4283163) {
        h->next ? h = h->next : 0;
        line = hist_but(line, h->data, x);
    }
    if (buf == 4348699) {
        h->prev ? h = h->prev : 0;
        line = hist_but(line, h->data, x);
    }
    if (buf == 4414235 && *x > 0) {
        printf("%c[1C", 27);
        (*x)--;
    }
    if (buf == 4479771 && line != NULL && mx_strlen(line) > *x) { 
        printf("%c[1D", 27);
        (*x)++;
    }
    *hs = h;
    fflush(stdout);
    return line;
}

static int check(t_ush *ush, int buf, char **l) {
    char *line = *l;

    if (buf == 10 || buf == 12 || (buf == 4 && 
        (line == NULL || !strlen(line))) || buf == 3) {
        if (buf == 3) {
            if (line != NULL)
                mx_strdel(l); 
            ush->last_return = 130;
        }
        else if (line != NULL && !strlen(line))
            mx_strdel(l);
        fflush(stdout);
        return 1;
    }
    return 0;
}

char *mx_read_stream(t_ush *ush, t_hst *h) {
    unsigned int buf = 0;
    char *line = NULL;
    int len = 0;
    int x = 0;
    t_hst *head = NULL;
    
    mx_push_f(&h, "\0"); 
    head = h; 
    while ((len = read(0, &buf, 4)) > 0) {
        if (len == 1) {
            if (check(ush, buf, &line) == 1) {
                buf == 4 ? line = mx_strdup("exit") : 0;
                break;
            }
            line = mx_stream(buf, line, &x);
        }
        (len > 1) ? line = button(&h, line, buf, &x) : 0;
        buf = 0;
    }
    mx_free_node(head);
    return line;
}
