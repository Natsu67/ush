#include "../inc/ush.h"

bool mx_opencheck(char *dirname, int *error, int flag_link) {
    DIR *dirdir;
    struct stat i;

    dirdir = opendir(dirname);
    if (errno == 13) {
        (*error) = 4;
        return false;
    }
    if (errno == 2) {
        (*error) = 1;
        return false;
    }
    if (dirdir == NULL) {
        (*error) = 2;
        return false;
    }
    lstat(dirname, &i);
    if (MX_ISLNK(i.st_mode) && flag_link == 1) {
        (*error) = 2;
        closedir(dirdir);
        return false;
    }
    closedir(dirdir);
    return true;
}
