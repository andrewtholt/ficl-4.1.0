#include <stdio.h>
#include <string.h>
#include "utils.h"


int main() {
    char path[1024];

    strcpy(path,"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin") ;

    char buffer[255];

    int rc = expandPath(path, "als", buffer);

    printf("rc=%d\n", rc);
    if( rc == 0) {
        printf("%s\n", buffer);
    }
}

