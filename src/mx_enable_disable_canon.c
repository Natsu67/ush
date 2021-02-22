#include "../inc/ush.h"

struct termios *mx_get_tty(void) {
    static struct termios tty;

    return &tty;
}

void mx_enable_canon(void) {
    struct termios tty;

    tcgetattr(STDIN_FILENO, mx_get_tty());
    tty = *mx_get_tty();
    tty.c_lflag &= ~(ICANON | ECHO | ISIG);
    tty.c_cc[VMIN] = 1;
    tcsetattr(STDIN_FILENO, TCSADRAIN, &tty);
}

void mx_disable_canon(void) {
    tcsetattr(STDIN_FILENO, TCSADRAIN, mx_get_tty());
}

void mx_init_signals(void) {
    sigset_t mask;

    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);
}
