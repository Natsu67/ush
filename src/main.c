#include "../inc/ush.h"

static void start_loop(t_ush *ush) {
    char *line = NULL;
    ush->hist = NULL;
    ush->env_set = mx_create_node(NULL);

    mx_signal_ignore();
    while (YARIK_PEREPISIVAYET_LS) {
        mx_enable_canon();
        write(1, "u$h> ", mx_strlen("u$h> "));
        line = mx_read_stream(ush, ush->hist);
        write (1, "\n", 1);
        if (line != NULL && mx_strlen(line) > 0) {
            mx_push_f(&ush->hist, line);
            mx_disable_canon();
            mx_parse(line, ush);
        }
        if (ush->exit >= 0)
            break ;
    }
    tcsetattr(0, TCSAFLUSH, &ush->savetty);
    mx_signal_def();
    mx_free_list2(&ush->env_set);
}

static void pipe_call(t_ush *ush) {
    int buf = 0;
    char *line = NULL;
    char *ch = NULL;

    ush->env_set = mx_create_node(NULL);
    while (read(STDIN_FILENO, &buf, 3) > 0) {
        ch = (char *)(&buf);
        line = mx_delit_fre(line, ch);
        buf = 0;
    }
    mx_parse(line, ush);
    mx_free_list2(&ush->env_set);
}

static void set_shell_lvl_up(void) {
    char *s;

    if (getppid() == 1)
        return;
    s = mx_itoa((atoi(getenv("SHLVL"))) + 1);
    setenv("SHLVL", s, 1);
    free(s);
}

static void set_def_env(t_ush *ush) {
    struct passwd *pw = getpwuid(getuid());
    extern char **environ;

    ush->pwd = getcwd(NULL, 0);
    ush->home = pw->pw_dir;
    if (environ[0] == NULL)
        setenv("SHLVL", "1", 1);
    setenv("PWD", ush->pwd, 1);
    setenv("HOME", ush->home, 1);
    setenv("LOGNAME", getlogin(), 1);
}

int main(void) {
    int ex = 0;
    t_ush *ush = (t_ush *)calloc(8, sizeof(t_ush));
    ush->jobs = mx_create_job(NULL, -1, -1, NULL);

    set_def_env(ush);
    set_shell_lvl_up();
    ush->exit = -1;
    if (isatty(0)) {
        mx_setup_term(ush);
        start_loop(ush);
        ex = ush->exit;
        mx_free_list(&ush->hist);
    }
    else
        pipe_call(ush);
    mx_free_jobs(&ush->jobs);
    mx_strdel(&ush->pwd);
    free(ush);
    exit(ex);
    return 0;
}
