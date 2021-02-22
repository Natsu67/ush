#include "../inc/ush.h"

static void signal_reload() {
    signal(SIGTSTP, SIG_DFL);
    signal(SIGINT, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
}

static int wife_check(char **args, t_jobs **jobs, int status, pid_t pid) {
    tcsetpgrp(0, getpid());
    tcsetpgrp(1, getpid());
    if (WIFSTOPPED(status)) {
        status = 146;
        mx_add_job(jobs, args, pid);
        write(1, "\n", 1);
    }
    else if (WIFSIGNALED(status)) {
        status = 130;
        write(1, "\n", 1);
    }
    else if (WIFEXITED(status)) 
        status =  WEXITSTATUS(status);
    errno = 0;
    return status;
}

int mx_straus_proc(char **args, t_jobs **jobs) {
    pid_t pid;
    int status = 0;

    pid = fork();
    if (pid == 0) {
        signal_reload();
        setpgid(pid, pid);
        if (isatty(0)) {
            tcsetpgrp(0, getpid());
            tcsetpgrp(1, getpid());
        }
        execvp(*args, args);
        mx_not_found(args[0], "ush: command");
        exit(127);
    }
    else 
        waitpid(pid, &status, WUNTRACED);
    return wife_check(args, jobs, status, pid);
}
