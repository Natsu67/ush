#include "../inc/ush.h"

static char optchar(char *optstring, bool *wordbeg, char **c) {
    if (*wordbeg) {
        if (**c != '-') {
            *c = NULL;
            *wordbeg = 1;
            return -1;
        }
        else if (!*(++(*c))) {
            *c = NULL;
            *wordbeg = 1;
            return -1;
        }
        else
            *wordbeg = 0;
    }
    if (mx_get_char_index(optstring, **c) == -1) {
        *c = NULL;
        *wordbeg = 1;
        return '?';
    }
    return **c;
}

char mx_getopt(int argc, char **argv, char *optstring, int *optind) {
    static bool wordbeg = 1;
    static char *c = NULL;
    
    if (c && !*(++c)) {
        (*optind)++;
        wordbeg = 1;
    }
    if (*optind == argc) {
        c = NULL;
        wordbeg = 1;
        return -1;
    }
    if (!c || !*c)
        c = argv[*optind];
    return optchar(optstring, &wordbeg, &c);
}
