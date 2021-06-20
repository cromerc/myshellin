#include <pwd.h>
#include <stdio.h>
#include <unistd.h>

char *get_user() {
    struct passwd *pass;
    pass = getpwuid(getuid());
    return pass->pw_name;
}
