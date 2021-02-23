#include "../inc/ush.h"

char *mx_strpart(char *str, int index) {
    if (str == NULL) return NULL;

    char *res = mx_strnew(mx_strlen(str) - index);
    for (int i = index, k = 0; str[i]; k++, i++) res[k] = str[i];

    return res;
}

int mx_not_found(char *args, char *str) {
    mx_printerr(str);
    mx_printerr(" not found: ");
    mx_printerr(args);
    mx_printerr("\n");
    return 0;
}

int mx_no_such(char *name, char *args, char *str) {
    mx_printerr(name);
    mx_printerr(": ");
    mx_printerr(args); 
    mx_printerr(": no such  ");
    mx_printerr(str);
    mx_printerr("\n");
    return 1;
}

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

bool mx_opencheck(char *dirname, int *error, int flag_link) {
    DIR *dir;
    struct stat i;

    dir = opendir(dirname);
    if (errno == 13) {
        (*error) = 4;
        return false;
    }
    if (errno == 2) {
        (*error) = 1;
        return false;
    }
    if (dir == NULL) {
        (*error) = 2;
        return false;
    }

    lstat(dirname, &i);
    if (MX_ISLNK(i.st_mode) && flag_link == 1) {
        (*error) = 2;
        closedir(dir);
        return false;
    }
    closedir(dir);
    
    return true;
}

char **mx_copy_dub_arr(char **args) {
    int size = 0;
    char **res = NULL;

    if(args == NULL) return res;
    
    for (; args[size]; size++);
    res = (char **)malloc((size + 1) * sizeof(char *));

    for (int i = 0; i <= size; i++) res[i] = NULL;

    for (int i = 0; i < size; i++) res[i] = mx_strjoin(res[i], args[i]);

    return res; 
}

struct termios *mx_get_tty() {
    static struct termios tty;

    return &tty;
}

void mx_enable_canonic_mode() {
    struct termios tty;

    tcgetattr(STDIN_FILENO, mx_get_tty());
    tty = *mx_get_tty();
    tty.c_lflag &= ~(ICANON | ECHO | ISIG);
    tty.c_cc[VMIN] = 1;
    tcsetattr(STDIN_FILENO, TCSADRAIN, &tty);
}

void mx_disable_canonic_mode() {
    tcsetattr(STDIN_FILENO, TCSADRAIN, mx_get_tty());
}

static void handle(int sig) { 
    pid_t pid = getpid();
    
    if (pid == 0 && sig == 2) {
        mx_printstr("^C\n");
        kill(pid, SIGINT);
    }

    if (pid == 0 && sig == 20) mx_printstr("^Z\n");
}

void mx_setup_term(t_ush *ush) {
    signal(SIGINT, handle);
    signal(SIGTSTP, handle);
    
    setvbuf(stdout, NULL, _IONBF, 0);
    tcgetattr(0, &ush->savetty);
}

static char optchar(char *opt_str, bool *check, char **str) {
    if (*check) {
        if (**str != '-') {
            *str = NULL;
            *check = 1;
            return -1;
        } else if (!*(++(*str))) {
            *str = NULL;
            *check = 1;
            return -1;
        } else {
            *check = 0;
        }
    }
    if (mx_get_char_index(opt_str, **str) == -1) {
        *str = NULL;
        *check = 1;
        return '?';
    }
    return **str;
}

char mx_getopt(int argc, char **argv, char *opt_str, int *opt_id) {
    static bool check = 1;
    static char *str = NULL;
    
    if (str && !*(++str)) {
        (*opt_id)++;
        check = 1;
    }
    if (*opt_id == argc) {
        str = NULL;
        check = 1;
        return -1;
    }

    if (!str || !*str) str = argv[*opt_id];

    return optchar(opt_str, &check, &str);
}

