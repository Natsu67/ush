#include "../inc/ush.h"

static void uns_var(t_list **env_set, int count, t_list **b) {
    t_list *front = *env_set;
    t_list *back = *b;

    if (count == 0) {
        if (front->next == NULL) {
            mx_free_list2(env_set);
            front = mx_create_node(NULL);
            *env_set = front;
        }
        else
            mx_pop_frontf(env_set);
        return ;
    }
    for (int i = 0; i < count - 1; i++)
        front = front->next;
    front->next = back->next;
    back->next = NULL;
    mx_free_list2(&back);
}

static bool coi(t_list **env, t_list **n, int count, char **tmp) {
    char **sub = NULL;
    t_list *f = *n;

    sub =  mx_strsplit(f->data, '=');
    if (mx_strcmp_null(sub[0], tmp[0]) == 0) {
        uns_var(env, count, &f);
        unsetenv(tmp[0]);
        mx_del_strarr(&sub);
        return true;
    }
    mx_del_strarr(&sub);
    return false;
}


static bool global_set(char *args) {
    extern char **environ;
    char **tmp = NULL;

    for (int i = 0; environ[i]; i++) {
        if (mx_get_substr_index(environ[i], args) >= 0) {
            tmp = mx_strsplit(environ[i], '=');
            if (mx_strcmp_null(tmp[0], args) == 0) {
               unsetenv(tmp[0]); 
               mx_del_strarr(&tmp);
               return true;
            }
            mx_del_strarr(&tmp);
        }
    }
    return false;
}

int mx_ush_unset(char **args, t_list **env_set) {
    char **tmp = NULL;
    int count;
    t_list *f = *env_set;

    for (int i = 1; args[i]; i++) {
        tmp = mx_strsplit(args [i], '=');
        count = 0;

        if (!global_set(tmp[0]) && (f != NULL && f->data != NULL)) {
            for ( ; f; f = f->next, count++) {
                if (mx_get_substr_index(f->data, tmp[0]) >= 0) {
                    if (coi(env_set, &f, count, tmp))
                        break ;
                } 
            }
        }
        mx_del_strarr(&tmp);
    }
    return 1;
}
