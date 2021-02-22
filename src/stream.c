#include "../inc/ush.h"

 static char *del_button(char *line) {
    char *res = mx_strnew(strlen(line) - 1);

    write(1, "\b \b", 3);
    strncpy(res, line, strlen(line) - 1);
    free(line);
    return res;
}

static char *del_check(char *line, int buf, int *x) {
    int index = 0;
    char *tmp = NULL;

    if (buf == 127 && line != NULL && strlen(line) > 0 && *x == 0)
        line = del_button(line);
    else if (buf == 127 && *x > 0 && *x < mx_strlen(line)) {
        index = strlen(line) - *x;
        tmp = mx_strpart(line, index);
        for (int i = *x; i > 0; i--)
            write(1, " ", 1);
        for (int i = 0; i <= *x; i++)
            line = del_button(line);
        mx_printstr(tmp);
        line = mx_cooljoin(line, tmp);
        for (int i = 0; i < *x; i++) {
            printf("%c[1D", 27);
            fflush(stdout);
        }
    }
    return line;
}

static char *edit_line(char *line, int *x, char *ch) {
    int index = 0;
    char *tmp = NULL;

    index = strlen(line) - *x;
    tmp = mx_strpart(line, index);
    for (int i = *x; i > 0; i--)
        write(1, " ", 1);
    for (int i = 0; i < *x; i++)
        line = del_button(line);
    printf("%s%s", ch, tmp);
    line = mx_delit_fre(line, ch);
    line = mx_cooljoin(line, tmp);
    for (int i = 0; i < *x; i++) {
        printf("%c[1D", 27);
        fflush(stdout);
    }
    return line;
}

static char *delete_but(char *line, int *x) {
    int index = 0;
    char *tmp = NULL;

    index = strlen(line) - *x;
    tmp = mx_strpart(line, index + 1);
    for (int i = 0; i < *x; i++)
        write(1, " ", 1);
    for (int i = 0; i < *x; i++)
        line = del_button(line);
    printf("%s", tmp);
    line = mx_cooljoin(line, tmp);
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
            write(1, ch, 4);
            line = mx_delit_fre(line, ch);
        }
    }
    fflush(stdout);
    return line;
}
