/*
** stub main for testing Ficl
** $Id: main.c,v 1.2 2010/09/10 09:01:28 asau Exp $
*/
/*
** Copyright (c) 1997-2001 John Sadler (john_sadler@alum.mit.edu)
** All rights reserved.
**
** Get the latest Ficl release at http://ficl.sourceforge.net
**
** I am interested in hearing from anyone who uses Ficl. If you have
** a problem, a success story, a defect, an enhancement request, or
** if you would like to contribute to the Ficl release, please
** contact me by email at the address above.
**
** L I C E N S E  and  D I S C L A I M E R
** 
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
** OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>

#include "ficl.h"
#include "extras.h"

char *loadPath;

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

extern int verbose;


int main(int argc, char **argv) {
    int returnValue = 0;
    char buffer[256];
    ficlVm *vm;
    ficlSystem *system;
    int ch;
    int tmp=0;
    verbose =-1;

    char *fileName=(char *)NULL;
    loadPath = getenv("FICL_PATH");

    char *cmd=NULL;
    
    ficlSystemInformation athFsi;

    memset(&athFsi,0,sizeof(struct ficlSystemInformation));

    athFsi.dictionarySize = FICL_DEFAULT_DICTIONARY_SIZE;

    while ((ch = getopt(argc,argv, "c:qh?d:f:sV")) != -1) {
        switch(ch) {
            case 'c':
//                cmd=(struct string *)strsave(optarg);
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
            case '?':
                usage();
                exit(0);
                break;
            case 'q':
                verbose=0;
                break;
        }
    }

//    system = ficlSystemCreate(NULL);
    system = ficlSystemCreate(&athFsi);

    ficlSystemCompileExtras(system);
    vm = ficlSystemCreateVm(system);


    if(verbose !=0) {
        returnValue = ficlVmEvaluate(vm, ".ver .( " __DATE__ " ) cr quit");
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

#ifdef MQTT
    sprintf(buffer,"defer mqtt-connect-callback\n");
    returnValue = ficlVmEvaluate(vm, buffer);
    sprintf(buffer,"defer mqtt-message-callback\n");
    returnValue = ficlVmEvaluate(vm, buffer);
#endif

    while (returnValue != FICL_VM_STATUS_USER_EXIT) {
        fputs(FICL_PROMPT, stdout);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) break;
        returnValue = ficlVmEvaluate(vm, buffer);
    }

    ficlSystemDestroy(system);
    return 0;
}

