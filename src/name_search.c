#include "../inc/ush.h"

int mx_name_search(char *tmp , t_jobs *jobs) {
    int res = -1;
    int count = 0;
    
    t_jobs *tmp_jobs = jobs;
    if (tmp_jobs->data != NULL) {
        while(tmp_jobs) {
            if (mx_get_substr_index(tmp_jobs->data[0], tmp) == 0) res = count;
            tmp_jobs = tmp_jobs->next;
            count++;
        }
    }

    return res;
}
