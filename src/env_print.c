#include "../inc/ush.h"

int mx_env_print(void) {
    extern char **environ;

    for (int i = 0; environ[i]; i++) {
        write(1, environ[i], mx_strlen(environ[i]));
        write(1, "\n", 1);
    }
    return 0;
}
