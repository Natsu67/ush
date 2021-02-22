#include "../inc/ush.h"

static int find_sin_q_pair(char *s, int *i, t_frmt_lst **arr) {
    for (int start = (*i)++; s[*i]; (*i)++) {
        if (s[*i] == '\'')  {
            mx_push_format(arr + SIN_Q, start, *i, NULL);
            return 0;
        }
    }
    fprintf(stderr, MX_ERR_PARSE_CLSSINQ);
    return -1;
}

int mx_check_single_quote(char *s, int *i, t_frmt_lst **arr) {
    if (s[*i] != '\'')
        return 1;
    if (((arr[TDOL_CMD] && (!arr[TDBL_Q] || (arr[TDBL_Q]
        && arr[TDBL_Q]->data->start < arr[TDOL_CMD]->data->start)))
        || !arr[TDBL_Q]) && find_sin_q_pair(s, i, arr) < 0) {
        return -1;  // changes position
    }
    return 0;
}
