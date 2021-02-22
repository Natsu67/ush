#include "../inc/ush.h"

t_jobs *mx_create_job(char **data, int num, pid_t pid, char *pwd)
{
    t_jobs *t = (t_jobs *)malloc(sizeof(t_jobs) * 4);
    t->data = mx_copy_dub_arr(data);
    t->pwd = mx_cut_str_forjob(pwd);
    t->num = num;
    t->index = 0;
    t->sign = '\0';
    t->pid = pid;
    t->next = NULL;
    return t;
}
