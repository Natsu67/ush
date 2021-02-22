#include "../inc/ush.h"

bool mx_check_parse(char *str) {
    t_frmt_lst *arr[NUM_Q] = {NULL};

    if (mx_get_format_str(str, arr) == -1) {
        mx_free_format_lists(arr);
        return 0;
    }
    mx_free_format_lists(arr);
    return 1;
}
