#include "../inc/ush.h"

void mx_replace_sub_str(char **s, int start, int end, char *replace) {
    if (!s || !*s) return;
    if (!replace) replace = "";

    char *new = calloc(1, strlen(*s) - (end - start) + strlen(replace));
    strncpy(new, *s, start);
    strcpy(new + start, replace);
    strcat(new + start, *s + end + 1);
    
    free(*s);
    *s = new;
}

void mx_quit_parse(char *line, t_ush *ush, int ret_val, t_frmt_lst **arr ) {
    mx_free_format_lists(arr);

    if(line) free(line);
    if(ret_val >= 0) ush->last_return = ret_val;
}

int mx_parse_exec(char *subline, t_ush *ush) {
    t_frmt_lst *arr[NUM_Q] = {NULL};
    char **argv;

    mx_get_format_str(subline, arr);
    mx_mark_quotes(subline, arr);
    mx_mark_chars(subline, arr);

    if (mx_handle_substitutions(&subline, arr, ush) == -1) {
        mx_quit_parse(NULL, ush, 1, arr);
        return -1;
    }

    subline = mx_clear_str(subline);
    argv = mx_strsplit(subline, M_DEL);
    int num = 1;
    if(mx_tilde_expansion(argv, ush) != -1) {
        num = mx_detect_builds(argv, ush);
    }

    mx_quit_parse(subline, ush, num, arr);
    mx_del_strarr(&argv);

    return 0;
}

int mx_semicolon_split(char *line, t_ush *ush, char ***subcommands) {
    t_frmt_lst *arr[NUM_Q] = {NULL};

    if (!line || mx_get_format_str(line, arr) < 0) {
        mx_quit_parse(line, ush, line ? 1 : -1, arr);
        return -1;
    }

    mx_mark_semicolon(line, arr);
    mx_free_format_lists(arr);
    *subcommands = mx_strsplit(line, M_SEMIC);
    return 0;
}

bool mx_check_parse(char *str) {
    t_frmt_lst *arr[NUM_Q] = {NULL};

    if (mx_get_format_str(str, arr) == -1) {
        mx_free_format_lists(arr);
        return 0;
    }
    mx_free_format_lists(arr);
    
    return 1;
}

int mx_parse(char *line, t_ush *ush) {
    char **subcommands = NULL;

    if (mx_semicolon_split(line, ush, &subcommands) == -1) return -1;

    for (char **s = subcommands; *s; s++) mx_parse_exec(strdup(*s), ush);

    mx_del_strarr(&subcommands);
    mx_quit_parse(line, ush, -1, NULL);

    return 0;
}
