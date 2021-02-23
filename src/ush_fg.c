#include "../inc/ush.h"

static int is_digit(char *args){
    for (int i = 1; args[i]; i++) {
        if (!mx_isdigit(args[i])) return 0;
    }
    return 1;
}

static int is_symbol(char *args, t_jobs **jobs) {
    if ((mx_isalpha(args[1]) || args[1] == '-' || args[1] == '+' || args[1] == '%'|| args[1] == '\0') && !mx_job_chars_find(args, jobs)) return 0;
    else if (is_digit(args) && !mx_job_num_find(args, jobs)) return 0;
    else {
        mx_not_found(args, "fg: job");
        return 0;
    }
    return 1; 
}

int mx_ush_fg(char **args, t_jobs **jobs) {
    if (args[1] == NULL) {
        if (mx_job_chars_find("%%", jobs)) return 1;
        else return 0; 
    }
    for(int i = 1; args[i]; i++) {
        if (mx_get_char_index(args[i], '%') == 0 && !is_symbol(args[i], jobs)) return 1;
        else if (mx_isalpha(args[i][0]) && !mx_job_chars_find(args[i], jobs)) return 1;
        else return mx_not_found(args[i], "fg: job");
    }
    return 0;
}

