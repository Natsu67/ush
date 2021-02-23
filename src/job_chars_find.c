#include "../inc/ush.h"

static bool curr_job(char *temp , t_jobs **jobs){
    t_jobs *j = *jobs;
    
    if (mx_strcmp(temp, "%") == 0 || mx_strcmp(temp, "+") == 0 || temp == NULL) { 
        if (j->data == NULL && j->num == -1) {
            write (1, "fg: no current job\n", 19);
            return false;
        }
        else if(j->next == NULL || j->sign == '+') mx_del_job(&j, 1, jobs);
        else {
            while(j->next->sign != '+') j = j->next;
            mx_del_job(&j, 2, jobs);
        }
    }

    return true;
}

static bool prev_job(char *temp , t_jobs **jobs){
    t_jobs *j = *jobs;

    if (mx_strcmp(temp, "-") == 0) {
        if (j->next == NULL) {
            write (1, "fg: no previous job\n", 20);
            return false;
        }
        else if(j->sign == '-') mx_del_job(jobs, 1, jobs);
        else {
            while(j->next->sign != '-') j = j->next;
            mx_del_job(&j, 2, jobs);
        }
    }

    return true;
}

static bool name_job(char *temp , t_jobs **jobs){
    t_jobs *j = *jobs;
    int num;

    if (mx_isalpha(temp[0])) {
        num = mx_name_search(temp , j);
        if(num == -1) {
            mx_not_found(temp, "fg: job"); 
            return false;
        }
        else if (num == 0) mx_del_job(&j, 1, jobs);
        else {
            while(num - 1){
                j = j->next;
                num--;
            }
            mx_del_job(&j, 2, jobs);
        }
    }

    return true;
}

bool mx_job_chars_find(char *args, t_jobs **jobs) {
    char *temp = mx_cut_str_forjob(args);
    bool flag = true;

    if (!curr_job(temp, jobs)) flag = false;
    if (!prev_job(temp, jobs)) flag = false;
    if (!name_job(temp, jobs)) flag = false;

    mx_strdel(&temp);
    return flag;
}
