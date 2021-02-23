#include "../inc/ush.h"

void mx_free_list(t_list **list) {
    t_list *temp_list = *list;

    while(temp_list) {
        t_list *tmp_parent = temp_list->next;

        if (temp_list->data != NULL) free(temp_list->data);
        free(temp_list);

        temp_list = NULL;
        temp_list = tmp_parent;
    }
}

void mx_free_jobs(t_jobs **jobs) {
    t_jobs *temp_jobs = *jobs;

    while(temp_jobs) {
        t_jobs *tmp_parent = temp_jobs->next;

        if (temp_jobs->data != NULL) mx_del_strarr(&temp_jobs->data);
        mx_strdel(&temp_jobs->pwd);

        temp_jobs->data = NULL;
        temp_jobs->num = 0;
        temp_jobs->pid = 0;
        temp_jobs->index = 0;
        temp_jobs->sign = '\0';
        
        free(temp_jobs);
        temp_jobs = NULL;
        temp_jobs = tmp_parent;
    }
}
