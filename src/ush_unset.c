#include "../inc/ush.h"

static void var_unset(t_list **env_set, int count, t_list **b) {
    t_list *front = *env_set;
    t_list *back = *b;

    if (count == 0) {
        if (front->next == NULL) {
            mx_free_list(env_set);
            front = mx_create_node(NULL);
            *env_set = front;
        }
        else mx_pop_front_list(env_set);
        return;
    }
    for (int i = 0; i < count - 1; i++) front = front->next;
    front->next = back->next;
    back->next = NULL;
    mx_free_list(&back);
}

static int list_unset(t_list **env, t_list **n, int count, char **temp) {
    char **sub = NULL;
    t_list *f = *n;

    sub =  mx_strsplit(f->data, '=');
    if (mx_strcmp(sub[0], temp[0]) == 0) {
        var_unset(env, count, &f);
        unsetenv(temp[0]);
        mx_del_strarr(&sub);
        return true;
    }
    mx_del_strarr(&sub);
    return 0;
}


static int global_set(char *args) {
    extern char **environ;
    char **temp = NULL;

    for (int i = 0; environ[i]; i++) {
        if (mx_get_substr_index(environ[i], args) >= 0) {
            temp = mx_strsplit(environ[i], '=');
            if (mx_strcmp(temp[0], args) == 0) {
               unsetenv(temp[0]); 
               mx_del_strarr(&temp);
               return 1;
            }
            mx_del_strarr(&temp);
        }
    }
    return 0;
}

int mx_ush_unset(char **args, t_list **env_set) {
    char **temp = NULL;
    int count;
    t_list *list = *env_set;

    args++;
    while(*args) {
        temp = mx_strsplit(*args, '=');
        count = 0;

        if (!global_set(temp[0]) && (list != NULL && list->data != NULL)) {
            while(list) {
                if (mx_get_substr_index(list->data, temp[0]) >= 0 && list_unset(env_set, &list, count, temp)) break;
                list = list->next;
                count++;
            }
        }
        mx_del_strarr(&temp);
        args++;
    }
    return 1;
}
