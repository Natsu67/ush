#include "../inc/ush.h"

t_jobs *mx_create_job(char **data, int num, pid_t pid, char *pwd) {
    t_jobs *job = (t_jobs *)malloc(4 * sizeof(t_jobs));
    job->data = mx_copy_dub_arr(data);
    job->pwd = mx_cut_str_forjob(pwd);
    job->num = num;
    job->index = 0;
    job->sign = '\0';
    job->pid = pid;
    job->next = NULL;
    return job;
}
