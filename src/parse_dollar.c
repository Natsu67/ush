#include "../inc/ush.h"

static int brace_pair_cycle(char *s, int *i, int start, t_frmt_lst **arr) {
    for (; s[*i]; (*i)++) {
        if (isalpha(s[*i]) || isdigit(s[*i]))
            continue;
        else if (s[*i] == '}') {
            mx_push_back_format(arr + DOL_BP, start, *i, NULL);
            return 0;
        }
        else {
            fprintf(stderr, MX_ERR_PARSE_BADSBN);
            break;
        }
    }
    fprintf(stderr, MX_ERR_PARSE_CLSBRC);
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
        fprintf(stderr, MX_ERR_PARSE_BADSBN);
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
    while (isalpha(s[*i + 1]) || isdigit(s[*i + 1]))
        (*i)++;
    if (*i - start == 0) {  // unescaped $ with no characters
        fprintf(stderr, MX_ERR_PARSE_UNESCDOL);
        return -1;
    }
    mx_push_back_format(arr + DOL_P, start, *i, NULL);
    return 0;
}

int mx_check_dollar(char *s, int *i, t_frmt_lst **arr) {
    if (s[*i] != '$')
        return 1;
    if (s[*i + 1] == '(') {
        mx_push_format(arr + TDOL_CMD, *i, -1, NULL);
        (*i)++;
        return 0;
    }
    else if (s[*i + 1] == '{')
        return find_brace_pair(s, i, arr);  // changes position
    else
        return find_dollar_param_end(s, i, arr);  // changes position
}

