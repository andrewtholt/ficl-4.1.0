#include <stdio.h>
#include <string.h>
#include "utils.h"


int main() {
    char path[1024];
    int rc=-1;

    strcpy(path,"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin") ;

    char buffer[255];

    rc = expandPath(path, "als", buffer);
    printf("als rc=%d\n", rc);
    if( rc == 0) {
        printf("%s\n", buffer);
    }

    rc = expandPath(path, "ls", buffer);
    printf("ls rc=%d\n", rc);
    if( rc == 0) {
        printf("%s\n", buffer);
    }
}

