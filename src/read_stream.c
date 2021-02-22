#include "../inc/ush.h"

static char *hist_but(char *line, char *data, int *tmp) {
    while(*tmp > 0) {
        printf("%c[1C", 27);
        fflush(stdout);
        (*tmp)--;
    }
    for (int i = 0; line != NULL && i < mx_strlen(line); i++) {
        write(1, "\b \b", 3);
    }
    mx_strdel(&line);
    write(1, data, mx_strlen(data));
    line = mx_strdup(data);
    return line;
}

static char *button(t_hst **hs, char *line, int buf, int *tmp) {
    t_hst *hst = *hs;

    if (buf == 4283163) {
        if(hst->next) hst = hst->next;
        line = hist_but(line, hst->data, tmp);
    }
    if (buf == 4348699) {
        if(hst->prev) hst = hst->prev;
        line = hist_but(line, hst->data, tmp);
    }
    if (buf == 4414235 && *tmp > 0) {
        printf("%c[1C", 27);
        (*tmp)--;
    }
    if (buf == 4479771 && line != NULL && mx_strlen(line) > *tmp) { 
        printf("%c[1D", 27);
        (*tmp)++;
    }
    *hs = hst;
    fflush(stdout);
    return line;
}

static int check(t_ush *ush, int buf, char **line) {
    char *tmp_line = *line;

    if (buf == 10 || buf == 12 || (buf == 4 && (tmp_line == NULL || !strlen(tmp_line))) || buf == 3) {
        if (buf == 3) {
            if (tmp_line != NULL) mx_strdel(line); 
            ush->last_return = 130;
        } else if (tmp_line != NULL && !strlen(tmp_line)) {
            mx_strdel(line);
        }
        fflush(stdout);
        return 1;
    }
    return 0;
}

char *mx_read_stream(t_ush *ush, t_hst *hst) {
    unsigned int buf = 0;
    char *line = NULL;
    int length = 0, tmp = 0;
    t_hst *head = NULL;
    
    mx_push_f(&hst, "\0"); 
    head = hst; 
    while ((length = read(0, &buf, 4)) > 0) {
        if (length == 1) {
            if (check(ush, buf, &line) == 1) {
                if(buf == 4) line = mx_strdup("exit");
                break;
            }
            line = mx_stream(buf, line, &tmp);
        }
        if(length > 1) line = button(&hst, line, buf, &tmp);
        buf = 0;
    }
    mx_free_node(head);
    return line;
}
