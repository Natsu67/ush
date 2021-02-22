#include "../inc/ush.h"

static void print_sec(t_jobs *jobs, char *flags) {
    t_jobs *j = jobs;

    write(1, " suspended  ", 12);
    for (int k = 0; j->data[k]; k++) {
        write (1, j->data[k], mx_strlen (j->data[k]));
        if (j->data[k + 1])
            write(1, " ", 1);
    }
    write(1, "\n", 1);
    if (flags != NULL && mx_get_char_index(flags, 'd') > 0) {
        write(1, "(pwd : ", 7);
        if (mx_strcmp_null(j->pwd, getenv("HOME")) == 0)
            write(1, "~", 1); 
        else
            write(1, j->pwd, mx_strlen(j->pwd));
        write(1, ")\n", 2);
    }
}

static void print_job(t_jobs *jobs, int num, char *flags) {
    t_jobs *j = jobs;
  
    while(num){
        j = j->next;
        num--;
    }
    write(1, "[", 1);
    mx_printint(j->num);
    write(1, "]", 1);
    write(1, "  ", 2);
    if (j->sign == '+')
        write(1, "+", 1);
    else if (j->sign == '-')
        write(1, "-", 1);
    else
       write(1, " ", 1);
    if (flags != NULL && (mx_get_char_index(flags, 'l') > 0 
        || mx_get_char_index(flags, 'p') > 0)){
        write(1, " ", 1);
        mx_printint(j->pid);
    }
    print_sec(j, flags); 
}

static bool check_flag(char *args) {
        for (int i = 1; args[i]; i++) {
            if (args[i] != 'd' && 
                args[i] != 'p' && args[i] != 'l'
                    && args[i] != 's') {
                        write(1, "jobs: bad option: -", 19);
                        mx_printchar(args[i]);
                        write(1, "\n", 1);
                        return false;
            }
        }
    return true;
}

static int ch_job(char **args, t_jobs *jobs, int i, char *flags) {
    int ind;
    t_jobs *j = jobs;

    if ((flags == NULL && args[1] != NULL) || (flags != NULL && args[2] != NULL)) { // если не нул
        //write(1, "+++++\n", 6);
        for ( ; args[i]; i++) {
            ind = mx_name_search(args[i], jobs);
            if (ind == -1) {
                mx_not_found(args[i], "jobs: job");
                return 1;
            }
            print_job(jobs, ind, flags);
        }
    }
    else { //если нул
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
