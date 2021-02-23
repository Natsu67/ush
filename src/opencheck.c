#include "../inc/ush.h"

bool mx_opencheck(char *dirname, int *error, int flag_link) {
    DIR *dir;
    struct stat i;

    dir = opendir(dirname);
    if (errno == 13) {
        (*error) = 4;
        return false;
    }
    if (errno == 2) {
        (*error) = 1;
        return false;
    }
    if (dir == NULL) {
        (*error) = 2;
        return false;
    }

    lstat(dirname, &i);
    if (MX_ISLNK(i.st_mode) && flag_link == 1) {
        (*error) = 2;
        closedir(dir);
        return false;
    }
    closedir(dir);
    
    return true;
}
