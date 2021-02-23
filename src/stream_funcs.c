#include "../inc/ush.h"

static char *hist_but(char *line, char *data, int *tmp) {
    while(*tmp > 0) {
        printf("%c[1C", 27);
        fflush(stdout);
        (*tmp)--;
    }
    for (int i = 0; line != NULL && i < mx_strlen(line); i++) {
        mx_printstr("\b \b");
    }
    mx_strdel(&line);
    mx_printstr(data);
    line = mx_strdup(data);
    return line;
}

static char *button(t_history **hs, char *line, int buf, int *tmp) {
    t_history *history = *hs;

    if (buf == 4283163) {
        if(history->next) history = history->next;
        line = hist_but(line, history->data, tmp);
    }
    if (buf == 4348699) {
        if(history->prev) history = history->prev;
        line = hist_but(line, history->data, tmp);
    }
    if (buf == 4414235 && *tmp > 0) {
        printf("%c[1C", 27);
        (*tmp)--;
    }
    if (buf == 4479771 && line != NULL && mx_strlen(line) > *tmp) { 
        printf("%c[1D", 27);
        (*tmp)++;
    }
    *hs = history;
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

static char *del_button(char *line) {
    char *res = mx_strnew(strlen(line) - 1);

    mx_printstr("\b \b");
    strncpy(res, line, strlen(line) - 1);
    free(line);
    return res;
}

static char *del_check(char *line, int buf, int *x) {
    int index = 0;
    char *temp = NULL;

    if (buf == 127 && line != NULL && strlen(line) > 0 && *x == 0)
        line = del_button(line);
    else if (buf == 127 && *x > 0 && *x < mx_strlen(line)) {
        index = strlen(line) - *x;
        temp = mx_strpart(line, index);
        for (int i = *x; i > 0; i--)
            mx_printstr(" ");
        for (int i = 0; i <= *x; i++)
            line = del_button(line);
        mx_printstr(temp);
        line = mx_strjoin(line, temp);
        for (int i = 0; i < *x; i++) {
            printf("%c[1D", 27);
            fflush(stdout);
        }
    }
    return line;
}

static char *edit_line(char *line, int *x, char *ch) {
    int index = 0;
    char *temp = NULL;

    index = strlen(line) - *x;
    temp = mx_strpart(line, index);
    for (int i = *x; i > 0; i--)
        mx_printstr(" ");
    for (int i = 0; i < *x; i++)
        line = del_button(line);
    printf("%s%s", ch, temp);
    line = mx_strjoin(line, ch);
    line = mx_strjoin(line, temp);
    for (int i = 0; i < *x; i++) {
        printf("%c[1D", 27);
        fflush(stdout);
    }
    return line;
}

static char *delete_but(char *line, int *x) {
    int index = 0;
    char *temp = NULL;

    index = strlen(line) - *x;
    temp = mx_strpart(line, index + 1);
    for (int i = 0; i < *x; i++)  mx_printstr(" ");
    for (int i = 0; i < *x; i++) line = del_button(line);
    printf("%s", temp);
    line = mx_strjoin(line, temp);
    for (int i = 0; i < *x - 1; i++) {
        printf("%c[1D", 27);
        fflush(stdout);
    }
    (*x)--;
    return line;
}

char *mx_stream(int buf, char *line, int *x) {
    char *ch = NULL;

    ch = (char *)(&buf);
    if (buf == 127)
        line = del_check(line, buf, x);
    if (buf == 4 && *x > 0)
        line = delete_but(line, x);
    else if (31 < buf && buf < 127) {
        if (*x != 0)
            line = edit_line(line, x, ch);
        else {
            mx_printstr(ch);
            line = mx_strjoin(line, ch);
        }
    }
    fflush(stdout);
    return line;
}

char *mx_read_stream(t_ush *ush, t_history *history) {
    unsigned int buf = 0;
    char *line = NULL;
    int length = 0, tmp = 0;
    t_history *head = NULL;
    
    mx_push_front_history(&history, "\0"); 
    head = history; 
    while ((length = read(0, &buf, 4)) > 0) {
        if (length == 1) {
            if (check(ush, buf, &line) == 1) {
                if(buf == 4) line = mx_strdup("exit");
                break;
            }
            line = mx_stream(buf, line, &tmp);
        }
        if(length > 1) line = button(&history, line, buf, &tmp);
        buf = 0;
    }
    mx_free_node(head);
    return line;
}
