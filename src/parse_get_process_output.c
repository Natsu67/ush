#include "../inc/ush.h"

static void end_reading(pid_t pid, int *pipe) {
    int status;
    
    close(pipe[0]);
    waitpid(pid, &status, WUNTRACED);
    tcsetpgrp(0, getpid());
    tcsetpgrp(1, getpid());
    errno = 0;
}

static char *read_output(pid_t pid, int *pipe) {
    int read_bytes = 0;
    char buf[BUFSIZ + 1] = "";
    char *tmp = NULL;
    char *ret = calloc(1, 1);

    close(pipe[1]);
    for (read_bytes = read(pipe[0], buf, BUFSIZ); read_bytes > 0;
        read_bytes = read(pipe[0], buf, BUFSIZ)) {
        tmp = mx_strjoin(ret, buf);
        free(ret);
        ret = tmp;
        memset(buf, 0, read_bytes);
    }
    end_reading(pid, pipe);
    return ret;
}

static char *process_output(char *str, int (*parse_p)(char *, t_ush *),
                            t_ush *ush) {
    pid_t pid;
    int p[2];  // pipe

    if (!mx_check_parse(str))
        return NULL;
    pipe(p);
    if ((pid = fork()) == -1) {
        perror("fork");
        return NULL; 
    }
    if (pid == 0) {
        tcsetpgrp(0, getpid());
        tcsetpgrp(1, getpid());
        setvbuf(stdout, NULL, _IONBF, 0);
        close(p[0]);
        dup2(p[1], 1);
        close(p[1]);
        if (parse_p(str, ush) == -1) {
            fprintf(stderr, MX_ERR_PARSE_CMDSBN);
            exit(1);
        }
        exit(0);
    }
    return read_output(pid, p);
}

char *mx_get_subst_outputs(char *str, t_ush *ush) {
    char **subcommands = {NULL};
    char *sum_output = NULL;

    if (mx_semicolon_split(str, ush, &subcommands) == -1) {  // parse errors
        mx_quit_parse(NULL, ush, 1, NULL);
        return NULL;
    }
    for (char **s = subcommands; *s; s++) {
        sum_output = mx_cooljoin(sum_output, process_output(*s,
                                 mx_parse_exec, ush));
    }
    if (!sum_output)
        sum_output = strdup("");
    mx_del_strarr(&subcommands);
    mx_quit_parse(NULL, ush, -1, NULL);
    return sum_output;
}

