#include "../inc/ush.h"

int mx_env_print(void) {
    extern char **environ;

    for (int i = 0; environ[i]; i++) {
        mx_printstr(environ[i]);
        mx_printstr("\n");
    }
    
    return 0;
}
