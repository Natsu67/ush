#include "../inc/ush.h"

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
