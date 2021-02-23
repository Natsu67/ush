#include "../inc/ush.h"

static bool curr_job(char *temp , t_jobs **jobs){
    t_jobs *cpy_jobs = *jobs;
    
    if (mx_strcmp(temp, "%") == 0 || mx_strcmp(temp, "+") == 0 || temp == NULL) { 
        if (cpy_jobs->data == NULL && cpy_jobs->num == -1) {
            write (1, "fg: no current job\n", 19);
            return false;
        } else if(cpy_jobs->next == NULL || cpy_jobs->sign == '+') {
            mx_del_job(&cpy_jobs, 1, jobs);
        } else {
            while(cpy_jobs->next->sign != '+') cpy_jobs = cpy_jobs->next;
            mx_del_job(&cpy_jobs, 2, jobs);
        }
    }

    return true;
}

static bool prev_job(char *temp , t_jobs **jobs){
    t_jobs *cpy_jobs = *jobs;

    if (mx_strcmp(temp, "-") == 0) {
        if (cpy_jobs->next == NULL) {
            write (1, "fg: no previous job\n", 20);
            return false;
        } else if(cpy_jobs->sign == '-') {
            mx_del_job(jobs, 1, jobs);
        } else {
            while(cpy_jobs->next->sign != '-') cpy_jobs = cpy_jobs->next;
            mx_del_job(&cpy_jobs, 2, jobs);
        }
    }

    return true;
}

static bool name_job(char *temp , t_jobs **jobs){
    t_jobs *cpy_jobs = *jobs;
    int num;

    if (mx_isalpha(temp[0])) {
        num = mx_name_search(temp , cpy_jobs);
        if(num == -1) {
            mx_not_found(temp, "fg: job"); 
            return false;
        } else if (num == 0) {
            mx_del_job(&cpy_jobs, 1, jobs);
        } else {
            while(num - 1){
                cpy_jobs = cpy_jobs->next;
                num--;
            }
            mx_del_job(&cpy_jobs, 2, jobs);
        }
    }

    return true;
}
static bool del_part(t_jobs **jobs, int num) {
    t_jobs *cpy_jobs = *jobs;

    while(cpy_jobs) { 
        if(cpy_jobs->next->num == num) {
            mx_del_job(&cpy_jobs, 2, jobs);
            return true;
        }
        cpy_jobs = cpy_jobs->next;
        
        if (cpy_jobs->next == NULL) break;
    }

    return false;
}

bool mx_job_num_find(char *args, t_jobs **jobs) {
    char *temp = mx_cut_str_forjob(args);
    int num = atoi(temp);
    t_jobs *cpy_jobs = *jobs;
    bool flag = true;

    if(cpy_jobs->num == num) mx_del_job(jobs, 1, jobs);
    else if (cpy_jobs->num > 0 && cpy_jobs->next != NULL) flag = del_part(jobs, num);
    else flag = false;

    mx_strdel(&temp);

    if (flag == false) mx_no_such("fg", args, "job");

    return flag;
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

char *mx_cut_str_forjob(char *args) {
    char *res = NULL;

    if (args == NULL || args[0] == '\0') return res;
    else if (args[0] != '%' || (args[0] == '/' && args[0] == '\0')) res = mx_strjoin(res, args);
    else res = mx_strjoin(res, &args[1]);

    return res;
}

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

