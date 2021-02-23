#include "../inc/ush.h"

void mx_signal_def() {
    signal(SIGINT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
}

void mx_signal_ignore() {
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
}

void mx_init_signals() {
    sigset_t mask;

    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);
}
