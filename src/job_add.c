#include "../inc/ush.h"

static void plus_on_min(t_jobs **jobs, int index) {
    t_jobs *job = *jobs;

    for ( ; job; job = job->next) {
        if (job->index != index && job->sign == '+') job->sign = '-';
        else if (job->index != index && job->sign == '-') job->sign = '\0';
    }
}

static int search_biggest(t_jobs *jobs) {
    int res = 0;
    for (; jobs; jobs = jobs->next) if (jobs->index > res) res = jobs->index;

    return res + 1;
}

static void insert(t_jobs **jobs, pid_t pid, char **args, t_jobs **f) {
    t_jobs *job = *jobs;
    t_jobs *temp = job->next;

    job->next = mx_create_job(args, job->num + 1, pid, getenv("PWD"));
    job->next->next = temp;
    job->next->index = search_biggest(job);
    job->next->sign = '+';
    plus_on_min(f, job->next->index);

    return;
}

static void to_body(t_jobs **jobs, char **args, pid_t pid) {
    t_jobs *job = *jobs;

    while (job) {
        if (job->next == NULL) {
            job->next = mx_create_job(args, job->num + 1, pid, getenv("PWD"));
            job->next->index = search_biggest(job);
            job->next->sign = '+';
            plus_on_min(jobs, job->next->index);
            break;
        }
        else if (job->next->num == job->num + 1) {
            job = job->next;
        }
        else {
            insert(&job, pid, args, jobs);
            break;
        }  
    }
}

void mx_add_job(t_jobs **jobs, char **args, pid_t pid) {
    t_jobs *job = *jobs;
    t_jobs *temp = NULL;

    if (job->data == NULL && job->num == -1) {
        job->data = mx_copy_dub_arr(args);
        job->index = 0;
        job->sign = '+';
        job->num = 1;
        job->pid = pid;
        job->pwd = mx_strdup(getenv("PWD"));
        return;
    }
    else if (job->num != 1) {
        temp = mx_create_job(args, 1, pid, getenv("PWD"));
        job->index = search_biggest(job);
        temp->sign = '+';
        temp->next = job;
        *jobs = temp;
        plus_on_min(jobs, 0);
        return;
    }
    
    to_body(jobs, args, pid);
}
