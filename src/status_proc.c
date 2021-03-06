#include "../inc/ush.h"

static int check(char **args, t_jobs **jobs, int status, pid_t pid) {
    tcsetpgrp(0, getpid());
    tcsetpgrp(1, getpid());
    if (WIFSTOPPED(status)) {
        status = 146;
        mx_add_job(jobs, args, pid);
        mx_printstr("\n");
    } else if (WIFSIGNALED(status)) {
        status = 130;
        mx_printstr("\n");
    } else if (WIFEXITED(status)) {
        status =  WEXITSTATUS(status);
    }
    errno = 0;
    return status;
}

int mx_status_proc(char **args, t_jobs **jobs) {
    pid_t pid;
    int status = 0;

    pid = fork();
    if (pid == 0) {
        mx_signal_reload();
        setpgid(pid, pid);
        if (isatty(0)) {
            tcsetpgrp(0, getpid());
            tcsetpgrp(1, getpid());
        }
        execvp(*args, args);
        mx_not_found(args[0], "ush: command");
        exit(127);
    } else {
        waitpid(pid, &status, WUNTRACED);
    }
    return check(args, jobs, status, pid);
}
