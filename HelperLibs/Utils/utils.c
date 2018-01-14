#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

// #include <linux/limits.h>

#include "utils.h"

int expandPath(char *dirList, char *fileName, char *buffer) {
    int rc = -1;

    char *ptr=NULL;

    char tmpSpace[4096];

    char *scratch;

    int len=strlen(dirList);

    if( len == 0) {
        return rc;
    }

    scratch=(char *)malloc( len );
    strcpy(scratch, dirList); 

    ptr = strtok( scratch, ":");

    do {
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

    if( scratch != NULL) {
        free(scratch);
    }

    return rc;
}

