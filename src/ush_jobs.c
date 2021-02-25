#include "../inc/ush.h"

static void print_job(t_jobs *jobs, int num, char *flags) {
    t_jobs *job = jobs;
  
    while(num){
        job = job->next;
        num--;
    }
    mx_printstr("[");
    mx_printint(job->num);
    mx_printstr("]  ");
    if (job->sign == '+' || job->sign == '-') mx_printchar(job->sign);
    else mx_printstr(" ");
    if (flags != NULL && (mx_get_char_index(flags, 'l') > 0 || mx_get_char_index(flags, 'p') > 0)){
        mx_printstr(" ");
        mx_printint(job->pid);
    }

    mx_printstr(" suspended  ");
    for (int k = 0; job->data[k]; k++) {
        mx_printstr(job->data[k]);
        if (job->data[k + 1]) mx_printstr(" ");
    }
    mx_printstr("\n");
    if (flags != NULL && mx_get_char_index(flags, 'd') > 0) {
        mx_printstr("(pwd : ");
        if (mx_strcmp(job->pwd, getenv("HOME")) == 0) mx_printstr("~"); 
        else mx_printstr(job->pwd);
        mx_printstr(")\n");
    } 
}

static int check_flag(char *args) {
    for (int i = 1; args[i]; i++) {
        if (args[i] != 'd' && args[i] != 'p' && args[i] != 'l' && args[i] != 's') {
            mx_printstr("jobs: bad option: -");
            mx_printchar(args[i]);
            mx_printstr("\n");
            return 0;
        }
    }
    return 1;
}

static int change_job(char **args, t_jobs *jobs, int i, char *flags) {
    int id;
    t_jobs *job = jobs;

    if (flags == NULL && ((args[1] != NULL || flags != NULL) && args[2] != NULL)) {
        for (; args[i]; i++) {
            id = mx_name_search(args[i], jobs);
            if (id == -1) {
                mx_not_found(args[i], "jobs: job");
                return 1;
            }
            print_job(jobs, id, flags);
        }
    } else { 
        i = 0;
        while(job) { 
            print_job(jobs, i, flags);  
            i++;
            job = job->next;
        }
    }
    return 0;
}

int mx_ush_jobs(char **args, t_jobs **jobs) {
    int i = 1;
    char *flags = NULL;
    int res = 1;
    t_jobs *job = *jobs;

    if (args[1] !=  NULL && args[1][0] ==  '-') {
        if (!check_flag(args[1])) return 1;
        i = 2;
        flags = mx_strjoin(flags, args[1]);
    }
    if ((args[1] == NULL && job->data == NULL) || (flags != NULL && args[2] == NULL && job->data == NULL)) {
        if (flags != NULL) mx_strdel(&flags);
        return 0;
    }
    res = change_job(args, job, i, flags);
    if (flags != NULL) mx_strdel(&flags);
    return res;
}
