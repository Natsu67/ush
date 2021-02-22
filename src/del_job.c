#include "../inc/ush.h"

static void plus_on_min(t_jobs **j, char a) {
    t_jobs *job = *j;
    int ind = -1;

    if (a == '+') {
        for ( ; job; job=job->next) {
            if (job->sign == '-') {
                job->sign = '+';
                ind = job->index - 1;
            }
            if (job->index == ind)
                job->sign = '-';
        }
    }
    if (a == '-') {
        for ( ; job; job=job->next) {
            if (job->sign == '+') {
                ind = job->index - 1;
            }
            if (job->index == ind)
                job->sign = '-';
        }
    }
}

static void reload(pid_t pid, char **args, t_jobs **jobs) {
    int status;

    tcsetpgrp(0, pid);
    tcsetpgrp(1, pid);
    kill(-pid, SIGCONT);
    waitpid(-pid, &status, WUNTRACED);
    tcsetpgrp(0, getpid());
    tcsetpgrp(1, getpid());
    if (WIFSTOPPED(status)) {
        mx_add_job(jobs, args, pid);
    }
    if (args != NULL)
        mx_del_strarr(&args);
    errno = 0;
}

static void del_body(t_jobs **jobs, t_jobs **first) {
    t_jobs *j = *jobs;
    t_jobs *del = j->next;
    char sign = '\0';
    char **data = mx_copy_dub_arr(del->data);
    int pid = del->pid;
    
    j->next = NULL;
    if (del->next != NULL)
        j->next = del->next;
    del->next = NULL;
    sign = del->sign;
    mx_free_jobs(&del);
    plus_on_min(jobs, sign);
    reload(pid, data, first);
}

static void forever_alone(t_jobs **jobs, t_jobs **f, char **data) {
    t_jobs *j = *jobs;
    char sign = '\0';
    int pid = j->pid;
    
    if (j->data != NULL) 
        mx_del_strarr(&j->data);
    mx_strdel(&j->pwd);
    j->data = NULL;
    j->num = -1;
    j->pid = -1;
    j->index = 0;
    sign = j->sign;
    j->sign = '\0';
    plus_on_min(jobs, sign);
    reload(pid, data, f);
}

void mx_del_job(t_jobs **jobs, int flag, t_jobs **first) {
    t_jobs *j = *jobs;
    t_jobs *del = j->next;
    char **data = NULL;
    int pid = j->pid;
    char sign = '\0';
    
    if (flag == 1) {
        data = mx_copy_dub_arr(j->data);
        if (j->next == NULL) //когда один остается
            forever_alone(jobs, first, data);
        else {
            j->next = NULL;
            sign = j->sign;
            mx_printchar(sign);
            mx_free_jobs(&j);
            *jobs = del;
            plus_on_min(jobs, sign);
            reload(pid, data, first);
        }
    }
    if (flag == 2) //тело
        del_body(jobs, first);
}
