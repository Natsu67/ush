#include "../inc/ush.h"

static void plus_on_min(t_jobs **jobs, char sign) {
    t_jobs *job = *jobs;
    int index = -1;

    if (sign == '+') {
        for ( ; job; job = job->next) {
            if (job->sign == '-') {
                job->sign = '+';
                index = job->index - 1;
            }
            if (job->index == index) job->sign = '-';
        }
    }

    if (sign == '-') {
        for ( ; job; job = job->next) {
            if (job->sign == '+') {
                index = job->index - 1;
            }
            if (job->index == index) job->sign = '-';
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
    t_jobs *cpy_job = *jobs;
    t_jobs *del = cpy_job->next;
    char sign = '\0';
    char **data = mx_copy_dub_arr(del->data);
    int pid = del->pid;
    
    cpy_job->next = NULL;
    if (del->next != NULL)
        cpy_job->next = del->next;
    del->next = NULL;
    sign = del->sign;

    mx_free_jobs(&del);
    plus_on_min(jobs, sign);
    reload(pid, data, first);
}

static void del_if_one(t_jobs **jobs, t_jobs **f, char **data) {
    t_jobs *cpy_job = *jobs;
    char sign = '\0';
    int pid = cpy_job->pid;
    
    if (cpy_job->data != NULL) 
        mx_del_strarr(&cpy_job->data);
    mx_strdel(&cpy_job->pwd);

    cpy_job->data = NULL;
    cpy_job->num = -1;
    cpy_job->pid = -1;
    cpy_job->index = 0;
    sign = cpy_job->sign;
    cpy_job->sign = '\0';

    plus_on_min(jobs, sign);
    reload(pid, data, f);
}

void mx_del_job(t_jobs **jobs, int flag, t_jobs **first) {
    t_jobs *cpy_job = *jobs;
    t_jobs *del = cpy_job->next;
    char **data = NULL;
    int pid = cpy_job->pid;
    char sign = '\0';
    
    if (flag == 1) {
        data = mx_copy_dub_arr(cpy_job->data);
        if (cpy_job->next == NULL)
            del_if_one(jobs, first, data);
        else {
            cpy_job->next = NULL;
            sign = cpy_job->sign;
            mx_printchar(sign);
            mx_free_jobs(&cpy_job);
            *jobs = del;
            plus_on_min(jobs, sign);
            reload(pid, data, first);
        }
    }
    if (flag == 2) del_body(jobs, first);
}
