#include "../inc/ush.h"

static bool del_part(t_jobs **jobs, int num) {
    t_jobs *j = *jobs;

    while(j) { 
        if(j->next->num == num) {
            mx_del_job(&j, 2, jobs);
            return true;
        }
        j = j->next;
        
        if (j->next == NULL) break;
    }

    return false;
}

bool mx_job_num_find(char *args, t_jobs **jobs) {
    char *temp = mx_cut_str_forjob(args);
    int num = atoi(temp);
    t_jobs *j = *jobs;
    bool flag = true;

    if(j->num == num) mx_del_job(jobs, 1, jobs);
    else if (j->num > 0 && j->next != NULL) flag = del_part(jobs, num);
    else flag = false;

    mx_strdel(&temp);

    if (flag == false) mx_no_such("fg", args, "job");

    return flag;
}
