#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>

#include "utils.h"

int expandPath(char *dirList, char *fileName, char *buffer) {
    int rc = -1;

    char *ptr=NULL;

    char tmpSpace[4096];

    int len=strlen(dirList);

    if( len == 0) {
        return rc;
    }

    char scratch[len];

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

    return rc;
}

static void displayLineHex(uint8_t *a) {
    int i;

    for(i=0;i<16;i++) {
        /*
           sprintf(outBuffer," %02x",*(a++));
           printf("%s",outBuffer);
           */
        printf(" %02x",*(a++));
    }    
}

static void displayLineAscii(uint8_t *a) {
    int i;

    printf(":");

    for(i=0;i<16;i++) {
        if( (*a < 0x20 ) || (*a > 0x80 )) {
            printf(".");
            a++;
        } else {
            printf("%c",*(a++));
        }
    }
    printf("\r\n");
}

void memDump(uint8_t *address, int length) {
    int lines=length/16;

    if(lines ==0 ) {
        lines=1;
    }
    printf("\r\n");

    int i=0;
    for( i = 0; i<length;i+=16) {
        /*
        sprintf(outBuffer,"%08x:", (uintptr_t)address);
        printf("%s", outBuffer);
        */
        printf("%08x:", (uintptr_t)address);

        displayLineHex( address );
        displayLineAscii( address );
        address +=16;
    }
}

