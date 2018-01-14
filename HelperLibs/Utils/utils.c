#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

// #include <linux/limits.h>

#include "utils.h"

int expandPath(char *dirList, char *fileName, char *buffer) {
    int rc = -1;

    char *ptr;

    char tmpSpace[4096];

    ptr = strtok( dirList, ":");

    do {
        printf("%s\n", ptr);

        strcpy(tmpSpace, ptr);
        strcat(tmpSpace,"/");
        strcat(tmpSpace,fileName);

        if ( access(tmpSpace, F_OK) == 0) {
            strcpy(buffer,tmpSpace);
            rc = 0;
            ptr = NULL;
        } else {
            ptr = strtok( NULL, ":");
        }
    } while(ptr != NULL);

    return rc;
}

