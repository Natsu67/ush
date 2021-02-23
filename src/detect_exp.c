#include "../inc/ush.h"

static char **contains_var(char *var, char *temp) {
    char **a;
    char **b;
    char **res = NULL;
    char **exp = mx_strsplit(temp, ' ');
  
    a = mx_strsplit(var, '=');
    for (int i = 1; exp[i]; i++) {
        b = mx_strsplit(exp[i], '=');
        if (mx_strcmp(b[0], a[0]) == 0) {
            res = (char **) malloc(sizeof(char *) * 3);
            res[0] = mx_strdup("export");
            res[1] = mx_strdup(a[0]);
            res[2] = NULL;
            mx_del_strarr(&a);
            mx_del_strarr(&b);
            mx_del_strarr(&exp); 
            return res;
        }
        mx_del_strarr(&b);
    }
    mx_del_strarr(&a);
    mx_del_strarr(&exp); 
    return res;  
}

static int find_exp_h(char *str, char *var, t_list **env_set) {
    char **temp = NULL;
    int res = 0;
    int flag = 0;
    char **exstr = NULL;

    temp = mx_strsplit(str, ';');
    for (int i = 0; temp[i]; i++) {
        if (mx_get_substr_index(temp[i], "export") >= 0) {
            exstr = contains_var(var, temp[i]);
            if (exstr != NULL) {
                flag = 1;
                if (mx_ush_export(exstr, env_set) == 1)
                    res = 1;
                mx_del_strarr(&exstr);
            } 
        }
    }
    mx_del_strarr(&temp);

    if (flag == 0) return 3; // did not find

    return res;
}

static bool global_set(char *args) {
    extern char **environ;
    char **temp = NULL;
    char **temp2 = mx_strsplit(args, '=');

    for (int i = 0; environ[i]; i++) {
        if (mx_get_substr_index(environ[i], temp2[0]) >= 0) {
            temp = mx_strsplit(environ[i], '=');
            if (mx_strcmp(temp[0], temp2[0]) == 0) {
               setenv(temp2[0], temp2[1], 1); 
               mx_del_strarr(&temp);
               mx_del_strarr(&temp2);
               return true;
            }
            mx_del_strarr(&temp);
        }
    }
    mx_del_strarr(&temp2);
    return false;
}

int mx_detect_exp(char **proc, t_history *start_h, t_list **env_set) {
    int temp;

    if(proc[0][0] == '=') return 3;

    if (mx_count_substr(proc[0], "=") == 1 && mx_isalpha(proc[0][mx_get_char_index(proc[0], '=') - 1])) {

        mx_env_in_list(env_set, proc[0]);

        for (t_history *h = start_h; h; h = h->next) {

            if (global_set(proc[0])) return 0;

            if (mx_get_substr_index(h->data, "export") >= 0) {
                temp = find_exp_h(h->data, proc[0], env_set);
                if (temp != 3) return temp;
                else continue;
            }
        }
        return 1; // a = b, but no export
    }
    return 3; // unknown command
}

