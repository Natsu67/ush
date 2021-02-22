#include "../inc/ush.h"

static void plus_on_min(t_jobs **j, int num) {
    t_jobs *job = *j;

    for ( ; job; job=job->next) {
        if (job->index != num && job->sign == '+')
            job->sign = '-';
        else if (job->index != num && job->sign == '-')
            job->sign = '\0';
    }
}

static int find_bigest(t_jobs *j) {
    int res = 0;

    for (; j; j=j->next) {
        if (j->index > res)
            res = j->index;
    }
    return res + 1;
}

static void insert(t_jobs **j, pid_t pid, char **args, t_jobs **f) {
    t_jobs *job = *j;
    t_jobs *tmp = job->next;

    job->next = mx_create_job(args, job->num + 1, 
        pid, getenv("PWD"));
    job->next->next = tmp;
    job->next->index = find_bigest(job);
    job->next->sign = '+';
    plus_on_min(f, job->next->index);
    return;
}

static void to_body(t_jobs **j, char **args, pid_t pid) {
    t_jobs *job = *j;

    while (job) {
        if (job->next == NULL) {
            job->next = mx_create_job(args, job->num + 1, 
                                      pid, getenv("PWD"));
            job->next->index = find_bigest(job);
            job->next->sign = '+';
            plus_on_min(j, job->next->index);
            break;
        }
        else if (job->next->num == job->num + 1) //до последней
            job = job->next;
        else { //пропущеный номер
            insert(&job, pid, args, j);
            break ;
        }  
    }
}

void mx_add_job(t_jobs **j, char **args, pid_t pid) {
    t_jobs *job = *j;
    t_jobs *tmp = NULL;

    if (job->data == NULL && job->num == -1) { //если пустой
        job->data = mx_copy_dub_arr(args);
        job->index = 0;
        job->sign = '+';
        job->num = 1;
        job->pid = pid;
        job->pwd = mx_strdup(getenv("PWD"));
        return ;
    }
    else if (job->num != 1) { //добавление вместо головы
        tmp = mx_create_job(args, 1, pid, getenv("PWD"));
        job->index = find_bigest(job);
        tmp->sign = '+';
        tmp->next = job;
        *j = tmp;
        plus_on_min(j, 0);
        return ;
    }
    to_body(j, args, pid);
}
