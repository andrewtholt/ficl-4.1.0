
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "ficl.h"
#include "extras.h"

    char *loadPath;
}
void usage() {
    printf("\nUsage: ficl <options> where options are: \n\n");
    printf("\t-c <cmd>\tExecute the command and exit.\n");
    printf("\t-d <size>\tSet the size of the dictionary.  Must be >= the default.\n");
    printf("\t-f <file>\tLoad this file at startup\n");
    printf("\t-h|?\t\tThis help.\n");
    printf("\t-q\t\tSupress startup messages.");

    printf("\n");

    printf("NOTES:\n");
    printf("\tThe environment variable FICL_PATH controls the locations searched for\n");
    printf("\tfiles.  It is a : seperated list of directories, e.g\n\n");

    printf("\t\t/usr/local/lib/ficl:/home/fred/ficl/lib:.\n\n");


    printf("\n\tFor Example;\n");
    printf("\n\t\texport FICL_PATH=\"/usr/local/lib/ficl:.\"\n\n");

    printf("\tTo display the options that were selected at build time, enter:\n\n");

    printf("\t\t.features\n\n");

}


static void athCPPTest(ficlVm *vm) {
    printf("C++ Main and extras works !!!\n");
}
void ficlSystemCompileCpp(ficlSystem *system) {
    ficlDictionary *dictionary = ficlSystemGetDictionary(system);

    ficlDictionarySetPrimitive(dictionary, (char *)"cpp-test", athCPPTest, FICL_WORD_DEFAULT);
}

int main() {
    char *loadPath;
    int returnValue = 0;
    char buffer[255];


    ficlSystem *fSystem;
    ficlVm *vm;

    char *fileName=(char *)NULL;
    loadPath = getenv("FICL_PATH");

    fSystem = ficlSystemCreate(NULL);

    ficlSystemCompileExtras(fSystem);
    ficlSystemCompileCpp(fSystem);

    vm = ficlSystemCreateVm(fSystem);


    while (returnValue != FICL_VM_STATUS_USER_EXIT) {
//        fputs(FICL_PROMPT, stdout);
        fputs("ok++> ", stdout);
        fflush(stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        returnValue = ficlVmEvaluate(vm, buffer);
    }   

    ficlSystemDestroy(fSystem);
    return 0;

}

