#include "../inc/ush.h"

static void handle(int sig) { 
    pid_t pid = getpid();
    
    if (pid == 0 && sig == 2) {
        write(1, "^C\n", 3);
        kill(pid, SIGINT);
    }
    if (pid == 0 && sig == 20)
        write(1, "^Z\n", 3);
}

void mx_setup_term(t_ush *ush) {
    signal(SIGINT, handle);
    signal(SIGTSTP, handle);
    setvbuf(stdout, NULL, _IONBF, 0);
    tcgetattr(0, &ush->savetty);
}
