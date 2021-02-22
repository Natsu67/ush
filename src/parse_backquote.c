#include "../inc/ush.h"

int mx_check_backquote(char *s, int *i, t_frmt_lst **arr) {
    if (s[*i] != '`')
        return 1;
    for (int start = (*i)++; s[*i]; (*i)++) {
        if (s[*i] == '\\') {
            if (arr[TSLASH])
                mx_pop_format(arr + TSLASH);
            else
                mx_push_format(arr + TSLASH, *i, -1, NULL);
        }
        else if (s[*i] == '`' && !arr[TSLASH]) {
            mx_push_format(arr + BCK_Q, start, *i, NULL);
            return 0;
        }
        else if (arr[TSLASH])
            mx_pop_format(arr + TSLASH);
    }
    fprintf(stderr, MX_ERR_PARSE_CLSBCKQ);
    return -1;
}
