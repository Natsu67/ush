#include "../inc/ush.h"

static void print_sec(t_jobs *jobs, char *flags) {
    t_jobs *j = jobs;

    mx_printstr(" suspended  ");
    for (int k = 0; j->data[k]; k++) {
        write (1, j->data[k], mx_strlen (j->data[k]));
        if (j->data[k + 1])
            mx_printstr(" ");
    }
    mx_printstr("\n");
    if (flags != NULL && mx_get_char_index(flags, 'd') > 0) {
        mx_printstr("(pwd : ");
        if (mx_strcmp(j->pwd, getenv("HOME")) == 0)
            mx_printstr("~"); 
        else
            mx_printstr(j->pwd);
        mx_printstr(")\n");
    }
}

static void print_job(t_jobs *jobs, int num, char *flags) {
    t_jobs *j = jobs;
  
    while(num){
        j = j->next;
        num--;
    }
    mx_printstr("[");
    mx_printint(j->num);
    mx_printstr("]");
    mx_printstr("  ");
    if (j->sign == '+')
        mx_printstr("+");
    else if (j->sign == '-')
        mx_printstr("-");
    else
       mx_printstr(" ");
    if (flags != NULL && (mx_get_char_index(flags, 'l') > 0 
        || mx_get_char_index(flags, 'p') > 0)){
        mx_printstr(" ");
        mx_printint(j->pid);
    }
    print_sec(j, flags); 
}

static bool check_flag(char *args) {
        for (int i = 1; args[i]; i++) {
            if (args[i] != 'd' && 
                args[i] != 'p' && args[i] != 'l'
                    && args[i] != 's') {
                        mx_printstr("jobs: bad option: -");
                        mx_printchar(args[i]);
                        mx_printstr("\n");
                        return false;
            }
        }
    return true;
}

static int ch_job(char **args, t_jobs *jobs, int i, char *flags) {
    int ind;
    t_jobs *j = jobs;

    if ((flags == NULL && args[1] != NULL) || (flags != NULL && args[2] != NULL)) {
        for ( ; args[i]; i++) {
            ind = mx_name_search(args[i], jobs);
            if (ind == -1) {
                mx_not_found(args[i], "jobs: job");
                return 1;
            }
            print_job(jobs, ind, flags);
        }
    }
    else {
        i = 0;
        for( ; j; i++, j = j->next)
            print_job(jobs, i, flags);   
    }
    return 0;
}

int mx_ush_jobs(char **args, t_jobs **jobs) {
    int i = 1;
    char *flags = NULL;
    int res = 1;
    t_jobs *j = *jobs;

    if (args[1] !=  NULL && args[1][0] ==  '-') {
        if (!check_flag(args[1]))
            return 1;
        i = 2;
        flags = mx_strjoin(flags, args[1]);
    }
    if ((args[1] == NULL && j->data == NULL) || 
        (flags != NULL && args[2] == NULL && j->data == NULL)) {
            if (flags != NULL)
                mx_strdel(&flags);
            return 0;
        }
    res = ch_job(args, j, i, flags);
    if (flags != NULL)
        mx_strdel(&flags);
    return res;
}
