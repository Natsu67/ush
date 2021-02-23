#include "../inc/ush.h"

int mx_check_double_quote(char *s, int *i, t_frmt_lst **arr) {
    if (s[*i] != '\"')
        return 1;
    if (arr[TDBL_Q]) {
        if (arr[TDOL_CMD]) {
            if (arr[TDBL_Q]->data->start > arr[TDOL_CMD]->data->start)
                mx_push_format(arr + DBL_Q, arr[TDBL_Q]->data->start, *i,
                               arr + TDBL_Q);
            else
                mx_push_format(arr + TDBL_Q, *i, -1, NULL);
        }
        else
            mx_push_format(arr + DBL_Q, arr[TDBL_Q]->data->start, *i,
                           arr + TDBL_Q);
    }
    else
        mx_push_format(arr + TDBL_Q, *i, -1, NULL);
    return 0;
}

int mx_check_open_paren(char *s, int *i, t_frmt_lst **arr) {
    if (s[*i] != '(')
        return 1;
    if (arr[TDBL_Q])
        return 0;
    fprintf(stderr, MX_ERR_PARSE_UNESCOPPAR);
    return -1;
    arr++;
}

int mx_check_close_paren(char *s, int *i, t_frmt_lst **arr) {
    if (s[*i] != ')')
        return 1;
    if (arr[TDOL_CMD] && (!arr[TDBL_Q] || (arr[TDBL_Q]
        && arr[TDBL_Q]->data->start < arr[TDOL_CMD]->data->start))) {
        mx_push_format(arr + DOL_CMD, arr[TDOL_CMD]->data->start, *i,
                       arr + TDOL_CMD);
        return 0;
    }
    else if (arr[TDBL_Q])
        return 0;
    else {
        fprintf(stderr, MX_ERR_PARSE_UNESCCLPAR);
        return -1;
    }
}

int mx_check_open_brace(char *s, int *i, t_frmt_lst **arr) {
    if (s[*i] != '{')
        return 1;
    if (arr[TDBL_Q])
        return 0;
    fprintf(stderr, MX_ERR_PARSE_UNESCOPBRC);
    return -1;
    arr++;  // to trick compiler
}

int mx_check_close_brace(char *s, int *i, t_frmt_lst **arr) {
    if (s[*i] != '}')
        return 1;
    if (arr[TDBL_Q])
        return 0;
    fprintf(stderr, MX_ERR_PARSE_UNESCCLBRC);
    return -1;
    arr++;  // to trick compiler
}
