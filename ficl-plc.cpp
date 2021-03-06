
extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "ficl.h"
#include "extras.h"
#include <readline/readline.h>
#include <readline/history.h>


    extern char *loadPath;
    extern int verbose;
    extern char *strsave(char *);
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

#ifdef PLC
#warning "PLC"
#include "plcBase.h"
#include "plcDatabase.h"
#endif

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

#ifdef PLC
static void athCreatePLC(ficlVm *vm) {

    plcDatabase *plc = new plcDatabase();

    ficlStackPushPointer(vm->dataStack, (void *) plc );

}

static void athDumpPLC(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);

    plc->plcDump();
}

static void athPLCsetHost(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int nlen = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    name[nlen] = '\0';

    bool failFlag=plc->setHost( name );
    ficlStackPushInteger( vm->dataStack, (int)failFlag );
}

static void athPLCsetPort(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int port = ficlStackPopInteger( vm->dataStack);

    int failFlag=plc->setPort( port );
    ficlStackPushInteger( vm->dataStack, (int)failFlag );
}

static void athPLCinit(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);

    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    bool failFlag=plc->initPlc(name);

    ficlStackPushInteger( vm->dataStack, (int)failFlag );
}

static void athPLCaddIO(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);

    int dlen = ficlStackPopInteger( vm->dataStack);
    char *dir = (char *) ficlStackPopPointer( vm->dataStack);

    int tlen = ficlStackPopInteger( vm->dataStack);
    char *topic = (char *) ficlStackPopPointer( vm->dataStack);

    int nlen = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->addIOPoint(name, topic, dir );
}

static void athPLCend(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int ms = ficlStackPopInteger( vm->dataStack);

    bool failFlag = plc->plcEnd(ms );
    ficlStackPushInteger( vm->dataStack, (int)failFlag );
}

static void athPLCld(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->Ld( name );
}

static void athPLCldr(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->Ldr( name );
}

static void athPLCldf(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->Ldf( name );
}

static void athPLCldn(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->Ldn( name );
}

static void athPLCand(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->And( name );
}

static void athPLCandn(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->Andn( name );
}

static void athPLCandr(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->Andr( name );
}

static void athPLCandf(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->Andf( name );
}

static void athPLCor(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->Or( name );
}

static void athPLCorn(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->Orn( name );
}

static void athPLCorr(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->Orr( name );
}

static void athPLCorf(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->Orf( name );
}

static void athPLCverbose(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int flag = ficlStackPopInteger( vm->dataStack);

    plc->setVerbose( (bool) flag);
}

static void athPLCout(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->Out( name );
}

static void athPLCoutn(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = ficlStackPopInteger( vm->dataStack);
    char *name = (char *) ficlStackPopPointer( vm->dataStack);

    plc->Outn( name );
}

static void athPLCrun(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);

//    plc->plcRun();
}

static void athPLCgetSem(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);

//    sem_t *mutex = plc->getSem();

//    ficlStackPushPointer(vm->dataStack, (void *) mutex );
}

// 
// Get Top plc stack entry DESTRUCTIVE
//
static void athPLCpop(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);

    bool t = plc->fromStack();

    int flag = (t) ? -1 : 0;

    ficlStackPushInteger( vm->dataStack, (int)flag );
}
// 
// Get Top plc stack entry NONE destructive
//
static void athPLCtos(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);

    bool t = plc->getTOS();

    int flag = (t) ? -1 : 0;

    ficlStackPushInteger( vm->dataStack, (int)flag );
}

static void athPLCpush(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int flag = (int )ficlStackPopInteger( vm->dataStack);

    bool t = (flag == 0) ? false : true ;

    plc->toStack(t);
}

static void athPLCdepth(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);

    int d = plc->stackSize();

    ficlStackPushInteger( vm->dataStack, (int)d );
}

static void athPLCsetDbName(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = (int )ficlStackPopInteger( vm->dataStack);
    char *db = (char * )ficlStackPopPointer( vm->dataStack);

    plc->setDBname( db );
}

static void athPLCdbSetup(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    bool clean = (bool )ficlStackPopInteger( vm->dataStack);

    plc->dbSetup( clean );
}

static void athPLCgetValue(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);
    int len = (int )ficlStackPopInteger( vm->dataStack);
    char *name = (char * )ficlStackPopPointer( vm->dataStack);

    // WARNING not sure of the side affects of static.
    static string value = plc->getValue( name );
    ficlStackPushPointer(vm->dataStack, (void *) value.c_str() );
    ficlStackPushInteger(vm->dataStack, (int ) value.length() );

}

static void athPLCsetValue(ficlVm *vm) {
    plcDatabase *plc = (plcDatabase *)ficlStackPopPointer( vm->dataStack);

    int len = (int )ficlStackPopInteger( vm->dataStack);
    char *name = (char * )ficlStackPopPointer( vm->dataStack);

    int vlen = (int )ficlStackPopInteger( vm->dataStack);
    char *value = (char * )ficlStackPopPointer( vm->dataStack);

    plc->setValue(name, value);

}
#endif

void ficlSystemCompileCpp(ficlSystem *system) {
    ficlDictionary *dictionary = ficlSystemGetDictionary(system);

    ficlDictionarySetPrimitive(dictionary, (char *)"cpp-test", athCPPTest, FICL_WORD_DEFAULT);
#ifdef PLC
    // 
    // PLC control functions.
    //
    ficlDictionarySetPrimitive(dictionary, (char *)"new-plc",  athCreatePLC, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-dump",  athDumpPLC, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-sethost",  athPLCsetHost, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-setport",  athPLCsetPort, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-init",  athPLCinit, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-verbose",  athPLCverbose, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-add-io",  athPLCaddIO, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-dbname!",  athPLCsetDbName, FICL_WORD_DEFAULT);

    ficlDictionarySetPrimitive(dictionary, (char *)"plc-value@",  athPLCgetValue, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-value!",  athPLCsetValue, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-db-setup", athPLCdbSetup, FICL_WORD_DEFAULT);
//    ficlDictionarySetPrimitive(dictionary, (char *)"plc-run",  athPLCrun, FICL_WORD_DEFAULT);
//    ficlDictionarySetPrimitive(dictionary, (char *)"plc-mutex",  athPLCgetSem, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-end",  athPLCend, FICL_WORD_DEFAULT);
    // 
    // PLC Logic functions.
    //
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-ld",  athPLCld,  FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-ldn", athPLCldn, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-ldr", athPLCldr, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-ldf", athPLCldf, FICL_WORD_DEFAULT);

    ficlDictionarySetPrimitive(dictionary, (char *)"plc-and",  athPLCand,  FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-andn", athPLCandn, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-andr", athPLCandr, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-andf", athPLCandf, FICL_WORD_DEFAULT);

    ficlDictionarySetPrimitive(dictionary, (char *)"plc-or",   athPLCor,  FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-orn",  athPLCorn, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-orr",  athPLCorr, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-orf",  athPLCorf, FICL_WORD_DEFAULT);

    ficlDictionarySetPrimitive(dictionary, (char *)"plc-out",  athPLCout,  FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-outn", athPLCoutn, FICL_WORD_DEFAULT);

    ficlDictionarySetPrimitive(dictionary, (char *)"plc-pop",  athPLCpop, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-push",  athPLCpush, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-tos",  athPLCtos, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"plc-depth",  athPLCdepth, FICL_WORD_DEFAULT);
#endif
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

