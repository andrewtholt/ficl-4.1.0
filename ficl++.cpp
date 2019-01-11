
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "ficl.h"
#include "extras.h"
#include <readline/readline.h>
#include <readline/history.h>


    extern char *loadPath;
    extern char *strsave(char *);
    extern int verbose;
}
void localCmds( char *ptr ) {
    char *historyFile ;
    char buffer[255];

    //    printf("Local >%s<\n",ptr);

    if(!strcmp( ptr, "save" )) {
#ifndef NOREADLINE
        historyFile = getenv("HOME");
        strcpy(buffer, historyFile );
        strcat(buffer, "/.ficl_history");
        write_history( buffer );
#endif
    } else if( !strcmp( ptr,"help")) {
        fprintf(stderr,"Help\n");
    }
}


#include <iostream>

using namespace std;

void usage() {
    printf("\nUsage: ficl++ <options> where options are: \n\n");
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
    cout << "C++ Main and extras works !!!" << endl;
}


void ficlSystemCompileCpp(ficlSystem *system) {
    ficlDictionary *dictionary = ficlSystemGetDictionary(system);

    ficlDictionarySetPrimitive(dictionary, (char *)"cpp-test", athCPPTest, FICL_WORD_DEFAULT);
}

int main(int argc, char *argv[]) {
    //    char *loadPath;
    int returnValue = 0;
    char buffer[255];


    ficlSystem *fSystem;
    ficlVm *vm;

    char *fileName=(char *)NULL;
    loadPath = getenv("FICL_PATH");

    if(loadPath == NULL ) {
        loadPath = strsave((char *)"/usr/local/lib/ficl:.");
    }
    char *cmd=NULL;

    ficlSystemInformation athFsi;

    memset(&athFsi,0,sizeof(struct ficlSystemInformation));
    athFsi.dictionarySize = FICL_DEFAULT_DICTIONARY_SIZE;

    int ch;
    int tmp=0;

    while ((ch = getopt(argc,argv, "c:qh?d:f:sV")) != -1) {
        switch(ch) {
            case 'c':
                cmd=strsave(optarg);
                printf("%s\n",(char *)cmd);
                break;
            case 'd':
                tmp=atoi( optarg );
                if ( tmp >= FICL_DEFAULT_DICTIONARY_SIZE ) {
                    athFsi.dictionarySize = tmp;
                } else {
                    fprintf(stderr,"Warning: new dictionary size ignored, must be larger than default\n");
                }
                break;
            case 'f':
                fileName=strsave(optarg);
                break;
            case 'h':
                usage();
                exit(0);
                break;
            case 'q':
                verbose=0;
                break;
        }
    }

    if( fileName == NULL ) {
        char tmpBuffer[255];
        char *home;

        home=getenv("HOME");
        sprintf(tmpBuffer,"%s/.ficlrc.fth",home);

        if ( access(tmpBuffer, R_OK) == 0) {
            fileName=strsave(tmpBuffer);
        }
    }

    bool useReadLine = true;
    char histFilename[255];
    memset(histFilename,0,255);

    if( isatty(0) ==0 ) {
        fprintf(stderr,"Not a tty\n");
        useReadLine = false;
    } else {
        fprintf(stderr,"Is a tty\n");
#ifndef NOREADLINE
        char *tmp;
        tmp = getenv("HOME");
        strcpy(histFilename, tmp );
        strcat(histFilename, "/.ficl_history");

        read_history( histFilename );
#else
#warning "No Readline"
        useReadLine = false;
#endif
    }

    fSystem = ficlSystemCreate(&athFsi);
    ficlSystemCompileExtras(fSystem);
    ficlSystemCompileCpp(fSystem);

    vm = ficlSystemCreateVm(fSystem);


    if(verbose !=0) {
        returnValue = ficlVmEvaluate(vm, (char *)".ver .( " __DATE__ " ) cr quit");
        // returnValue = ficlVmEvaluate(vm, ".ver cr quit");
    }

    /*
     ** load files specified on command-line
     */

    if( fileName != NULL ) {
        memset(buffer,0,255);
        if( verbose == 0) {
            sprintf(buffer, "load %s\n cr", fileName );
        } else {
            sprintf(buffer, ".( loading %s ) cr load %s\n cr", fileName, fileName );
        }
        returnValue = ficlVmEvaluate(vm, buffer);
    }   

    if( cmd != NULL ) {
        returnValue = ficlVmEvaluate(vm, (char *)cmd);
        exit(returnValue);
    }

    char *buf;

    while (returnValue != FICL_VM_STATUS_USER_EXIT) {
#ifndef NOREADLINE
        buf = readline(FICL_PROMPT);

        if( buf[0] == '^' ) {
            localCmds( &buf[1] );
        } else {
            returnValue = ficlVmEvaluate(vm, buf);
        }

        if (buf[0]!=0) {
            add_history(buf);
        }
        free(buf);
#else
        fputs(FICL_PROMPT, stdout);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        returnValue = ficlVmEvaluate(vm, buffer);
#endif
    }

#ifndef NOREADLINE
    if(useReadLine) {
        write_history( histFilename );
    }
#endif
    ficlSystemDestroy(fSystem);
    return 0;
}

