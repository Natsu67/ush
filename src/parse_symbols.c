#include "../inc/ush.h"

int mx_check_double_quote(char *s, int *i, t_frmt_lst **arr) {
    if (s[*i] != '\"') return 1;

    if (arr[TDBL_Q]) {
        if (arr[TDOL_CMD]) {
            if (arr[TDBL_Q]->data->start > arr[TDOL_CMD]->data->start) mx_push_format(arr + DBL_Q, arr[TDBL_Q]->data->start, *i, arr + TDBL_Q);
            else mx_push_format(arr + TDBL_Q, *i, -1, NULL);
        } else {
            mx_push_format(arr + DBL_Q, arr[TDBL_Q]->data->start, *i, arr + TDBL_Q);
        }
    } else {
        mx_push_format(arr + TDBL_Q, *i, -1, NULL);
    }

    return 0;
}

int mx_check_open_paren(char *s, int *i, t_frmt_lst **arr) {
    if(arr) {}
    if (s[*i] != '(') return 1;
    if (arr[TDBL_Q]) return 0;

    fprintf(stderr, "ush: unescaped (\n");
    return -1;
}

int mx_check_close_paren(char *s, int *i, t_frmt_lst **arr) {
    if (s[*i] != ')') return 1;

    if (arr[TDOL_CMD] && (!arr[TDBL_Q] || (arr[TDBL_Q] && arr[TDBL_Q]->data->start < arr[TDOL_CMD]->data->start))) {
        mx_push_format(arr + DOL_CMD, arr[TDOL_CMD]->data->start, *i, arr + TDOL_CMD);
        return 0;
    }
    else if (arr[TDBL_Q]) return 0;
    else {
        fprintf(stderr, "ush: unescaped )\n");
        return -1;
    }
}

int mx_check_open_brace(char *s, int *i, t_frmt_lst **arr) {
    if(arr) {}
    if (s[*i] != '{') return 1;
    if (arr[TDBL_Q]) return 0;

    fprintf(stderr, "ush: unescaped {\n");
    return -1;
}

int mx_check_close_brace(char *s, int *i, t_frmt_lst **arr) {
    if(arr) {}
    if (s[*i] != '}') return 1;
    if (arr[TDBL_Q]) return 0;

    fprintf(stderr, "ush: unescaped }\n");
    return -1;
}

static int brace_pair_cycle(char *s, int *i, int start, t_frmt_lst **arr) {
    for (; s[*i]; (*i)++) {
        if (isalpha(s[*i]) || isdigit(s[*i])) {
            continue;
        } else if (s[*i] == '}') {
            mx_push_back_format(arr + DOL_BP, start, *i, NULL);
            return 0;
        } else {
            fprintf(stderr, "ush: bad substitution\n");
            break;
        }
    }
    fprintf(stderr, "ush: closing brace expected\n");

    return -1;
}

static int find_brace_pair(char *s, int *i, t_frmt_lst **arr) {
    int start = *i;

    *i += 2;
    if (s[*i] == '?') {
        if (s[*i + 1] == '}') {
            mx_push_back_format(arr + DOL_BP, start, *i + 1, NULL);
            return 0;
        }
        fprintf(stderr, "ush: bad substitution\n");
        return -1;
    }

    return brace_pair_cycle(s, i, start, arr);
}

static int find_dollar_param_end(char *s, int *i, t_frmt_lst **arr) {
    int start = *i;

    if (s[*i + 1] == '?') {
        mx_push_back_format(arr + DOL_P, start, *i + 1, NULL);
        return 0;
    }

    while (isalpha(s[*i + 1]) || isdigit(s[*i + 1])) (*i)++;

    if (*i - start == 0) {
        fprintf(stderr, "ush: unescaped dollar sign\n");
        return -1;
    }
    mx_push_back_format(arr + DOL_P, start, *i, NULL);

    return 0;
}

int mx_check_dollar(char *s, int *i, t_frmt_lst **arr) {
    if (s[*i] != '$') return 1;
    
    if (s[*i + 1] == '(') {
        mx_push_format(arr + TDOL_CMD, *i, -1, NULL);
        (*i)++;
        return 0;
    }
    else if (s[*i + 1] == '{') return find_brace_pair(s, i, arr);
    else return find_dollar_param_end(s, i, arr);
}

static int tilde(char **str, t_ush *ush) {
    char *res = NULL;
    char *tmp = NULL;
    char **str_arr = mx_strsplit(*str, '/');

    if (strcmp(str_arr[0], "~+") == 0) {
        mx_replace_sub_str(str, 0, 1, ush->pwd);
        return 0;
    } else if (strcmp(str_arr[0], "~-") == 0) {
        mx_replace_sub_str(str, 0, 1, ush->home);
        return 0;
    } else if (strcmp(str_arr[0], "~") == 0){
        mx_replace_sub_str(str, 0, 0, ush->home);
        return 0;
    }

    if (!getpwnam((tmp = strndup(*str + 1, strlen(str_arr[0]) - 1)))) {
        fprintf(stderr, "ush: no such user or named directory: " "%s\n", tmp);
        mx_del_strarr(&str_arr);
        free(tmp);
        return -1;
    }

    mx_replace_sub_str(str, 0, strlen(tmp), (res = mx_strjoin("/Users/", tmp)));
    mx_del_strarr(&str_arr);
    free(res);
    free(tmp);
    return 0;
}

int mx_tilde_expansion(char **argv, t_ush *ush) {
    if (!argv || !*argv) return 0;

    for (char **str_arr = argv + 1; *str_arr; str_arr++) {
        if (**str_arr != '~') continue;

        if (tilde(str_arr, ush) == -1) return -1;
    }
    return 0;
} 

int mx_check_backquote(char *s, int *i, t_frmt_lst **arr) {
    if (s[*i] != '`') return 1;

    for (int start = (*i)++; s[*i]; (*i)++) {
        if (s[*i] == '\\') {
            if (arr[TSLASH]) mx_pop_format(arr + TSLASH);
            else mx_push_format(arr + TSLASH, *i, -1, NULL);
        } else if (s[*i] == '`' && !arr[TSLASH]) {
            mx_push_format(arr + BCK_Q, start, *i, NULL);
            return 0;
        } else if (arr[TSLASH]) {
            mx_pop_format(arr + TSLASH);
        }
    }
    fprintf(stderr, "ush: closing backquote expected\n");

    return -1;
}

static int find_pair(char *s, int *i, t_frmt_lst **arr) {
    for (int start = (*i)++; s[*i]; (*i)++) {
        if (s[*i] == '\'')  {
            mx_push_format(arr + SIN_Q, start, *i, NULL);
            return 0;
        }
    }
    fprintf(stderr, "ush: closing single quote expected\n");

    return -1;
}

int mx_check_single_quote(char *s, int *i, t_frmt_lst **arr) {
    if (s[*i] != '\'') return 1;
    
    if (((arr[TDOL_CMD] && (!arr[TDBL_Q] || (arr[TDBL_Q]
        && arr[TDBL_Q]->data->start < arr[TDOL_CMD]->data->start)))
        || !arr[TDBL_Q]) && find_pair(s, i, arr) < 0) {
        return -1;
    }
    
    return 0;
}
