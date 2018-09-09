#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>

#include <errno.h>

#include "ficl.h"
#include "extras.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>


int verbose;
char *loadPath;
#ifndef FICL_ANSI

/*
 ** Ficl interface to _getcwd (Win32)
 ** Prints the current working directory using the VM's 
 ** textOut method...
 */
static void ficlPrimitiveGetCwd(ficlVm *vm)
{
    char *directory;

    directory = getcwd(NULL, 80);
    ficlVmTextOut(vm, directory);
    ficlVmTextOut(vm, "\n");
    free(directory);
    return;
}



/*
 ** Ficl interface to _chdir (Win32)
 ** Gets a newline (or NULL) delimited string from the input
 ** and feeds it to the Win32 chdir function...
 ** Example:
 **    cd c:\tmp
 */
static void ficlPrimitiveChDir(ficlVm *vm)
{
    ficlCountedString *counted = (ficlCountedString *)vm->pad;
    ficlVmGetString(vm, counted, '\n');
    if (counted->length > 0)
    {
        int err = chdir(counted->text);
        if (err)
        {
            ficlVmTextOut(vm, "Error: path not found\n");
            ficlVmThrow(vm, FICL_VM_STATUS_QUIT);
        }
    }
    else
    {
        ficlVmTextOut(vm, "Warning (chdir): nothing happened\n");
    }
    return;
}



static void ficlPrimitiveClock(ficlVm *vm)
{
    clock_t now = clock();
    ficlStackPushUnsigned(vm->dataStack, (ficlUnsigned)now);
    return;
}

char *strsave( char *s ) {
    char *p = (char *)NULL;

    p = ficlMalloc(strlen(s)+1);
    strcpy(p,s);

    return(p);
}


static void athStrsave(ficlVm * vm) {
    int             len;
    char           *str;
    char           *p;

    len = ficlStackPopInteger(vm->dataStack);
    str = ficlStackPopPointer(vm->dataStack);

    str[len] = '\0';
    p = (char *) strsave(str);

    ficlStackPushPointer(vm->dataStack, p);
    ficlStackPushInteger(vm->dataStack, len);
}

static void athStrFree(ficlVm * vm) {
    int             len;
    char           *str;
    char           *p;

    len = ficlStackPopInteger(vm->dataStack);
    str = ficlStackPopPointer(vm->dataStack);

    if( str != NULL) {
        memset(str, 0, len);
        ficlFree( str );
    }
}

#endif /* FICL_ANSI */

/*
 ** Ficl add-in to load a text file and execute it...
 ** Cheesy, but illustrative.
 ** Line oriented... filename is newline (or NULL) delimited.
 ** Example:
 **    load test.f
 */

char *pathToFile(char *fname) {

    int             fd;
    extern char    *loadPath;
    char            path[255];
    char            scr[255];
    char           *scratch;
    char           *tok;


    if ((loadPath == (char *) NULL) || (*fname == '/') || (*fname == '.'))
        return (fname);

    strcpy(path, loadPath);

    tok = (char *) strtok(path, ":");


    for (; tok != (char *) NULL;) {
        strcpy(scr, tok);
        strcat(scr, "/");
        strcat(scr, fname);

        scratch = strsave(scr);

        fd = open(scratch, O_RDONLY);

        if (fd >= 0) {
            close(fd);
            return (scratch);
        }
        tok = (char *) strtok(NULL, ":");
    }   
    return (NULL);
}

static void ficlDollarPrimitiveLoad(ficlVm * vm) {
    char            buffer[BUFFER_SIZE];
    char            filename[BUFFER_SIZE];
    char            fullName[255];

    char           *scratch;
    ficlCountedString *counted = (ficlCountedString *) filename;
    int             line = 0;
    FILE           *f;
    int             result = 0;
    ficlCell        oldSourceId;
    ficlString      s;
    int             nameLen;
    char           *name;
    char *ptr=(char *)NULL;

    nameLen = ficlStackPopInteger(vm->dataStack);
    ptr=ficlStackPopPointer(vm->dataStack);
    name=strtok(ptr," ");
    name[nameLen] = '\0';

    scratch = pathToFile(name);

    if (scratch == (char *) NULL) {
        sprintf(buffer, "File not found :%s", name);
        ficlVmTextOut(vm, buffer);
        ficlVmTextOut(vm, FICL_COUNTED_STRING_GET_POINTER(*counted));
        ficlVmTextOut(vm, "\n");
        ficlVmThrow(vm, FICL_VM_STATUS_QUIT);
    } else {
        strcpy(fullName, scratch);
        strcpy(buffer, scratch);
    }
    /*
     *      ** get the file's size and make sure it exists
     *           */
    f = fopen(fullName, "r");
    if (!f) {
        sprintf(buffer, "Unable to open file %s", name);
        ficlVmTextOut(vm, buffer);
        ficlVmTextOut(vm, FICL_COUNTED_STRING_GET_POINTER(*counted));
        ficlVmTextOut(vm, "\n");
        ficlVmThrow(vm, FICL_VM_STATUS_QUIT);
    }
    oldSourceId = vm->sourceId;
    vm->sourceId.p = (void *) f;

    scratch = basename( buffer );
    char xfer[255];
    strcpy(xfer, scratch);

    ficlString fName = { 0 };
    FICL_STRING_SET_FROM_CSTRING(fName, xfer);
    ficlWord *wPtr = ficlDictionaryLookup(ficlVmGetDictionary(vm), fName);

    if( wPtr == NULL) {
        sprintf( buffer, "-1 constant %s", fName.text);
        result = ficlVmEvaluate(vm, buffer);
        memset(buffer,0,sizeof(buffer));
    } else {
        printf("%s already loaded\n", fName.text);
        fclose(f);
        return;
    }

    /* feed each line to ficlExec */
    while (fgets(buffer, BUFFER_SIZE, f)) {
        int length = strlen(buffer) - 1;

        line++;
        if (length <= 0)
            continue;

        if (buffer[length] == '\n')
            buffer[length--] = '\0';

        FICL_STRING_SET_POINTER(s, buffer);
        FICL_STRING_SET_LENGTH(s, length + 1);
        result = ficlVmExecuteString(vm, s);
        /* handle "bye" in loaded files. --lch */
        switch (result) {
            case FICL_VM_STATUS_OUT_OF_TEXT:
                break;
            case FICL_VM_STATUS_USER_EXIT:
                exit(0);
                break;

            default:
                vm->sourceId = oldSourceId;
                fclose(f);
                ficlVmThrowError(vm, "Error loading file <%s> line %d", FICL_COUNTED_STRING_GET_POINTER(*counted), line);
                break;
        }
    }
    /*
     ** Pass an empty line with SOURCE-ID == -1 to flush
     ** any pending REFILLs (as required by FILE wordset)
     */
    vm->sourceId.i = -1;
    FICL_STRING_SET_FROM_CSTRING(s, "");
    ficlVmExecuteString(vm, s);

    vm->sourceId = oldSourceId;
    fclose(f);

    /* handle "bye" in loaded files. --lch */
    if (result == FICL_VM_STATUS_USER_EXIT)
        ficlVmThrow(vm, FICL_VM_STATUS_USER_EXIT);
    return;
}

static void ficlPrimitiveLoad(ficlVm * vm) {

    char            filename[BUFFER_SIZE];

    extern char    *loadPath;
    char           *name;

    ficlCountedString *counted = (ficlCountedString *) filename;
    ficlVmGetString(vm, counted, '\n');

    if (FICL_COUNTED_STRING_GET_LENGTH(*counted) <= 0) {
        ficlVmTextOut(vm, "Warning (load): nothing happened\n");
        return;
    }
    name = FICL_COUNTED_STRING_GET_POINTER(*counted);

    ficlStackPushPointer(vm->dataStack, name);
    ficlStackPushInteger(vm->dataStack, FICL_COUNTED_STRING_GET_LENGTH(*counted));
    ficlDollarPrimitiveLoad(vm);
}
/*
 ** Ficl interface to system (ANSI)
 ** Gets a newline (or NULL) delimited string from the input
 ** and feeds it to the ANSI system function...
 ** Example:
 **    system del *.*
 **    \ ouch!
 */
static void ficlPrimitiveSystem(ficlVm *vm)
{
    ficlCountedString *counted = (ficlCountedString *)vm->pad;

    ficlVmGetString(vm, counted, '\n');
    if (FICL_COUNTED_STRING_GET_LENGTH(*counted) > 0)
    {
        int returnValue = system(FICL_COUNTED_STRING_GET_POINTER(*counted));
        if (returnValue)
        {
            sprintf(vm->pad, "System call returned %d\n", returnValue);
            ficlVmTextOut(vm, vm->pad);
            ficlVmThrow(vm, FICL_VM_STATUS_QUIT);
        }
    }
    else
    {
        ficlVmTextOut(vm, "Warning (system): nothing happened\n");
    }
    return;
}



/*
 ** Ficl add-in to load a text file and execute it...
 ** Cheesy, but illustrative.
 ** Line oriented... filename is newline (or NULL) delimited.
 ** Example:
 **    load test.f
 */

#if 0
#define BUFFER_SIZE 256
static void ficlPrimitiveLoad(ficlVm *vm)
{
    char    buffer[BUFFER_SIZE];
    char    filename[BUFFER_SIZE];
    ficlCountedString *counted = (ficlCountedString *)filename;
    int     line = 0;
    FILE   *f;
    int     result = 0;
    ficlCell    oldSourceId;
    ficlString s;

    ficlVmGetString(vm, counted, '\n');

    if (FICL_COUNTED_STRING_GET_LENGTH(*counted) <= 0)
    {
        ficlVmTextOut(vm, "Warning (load): nothing happened\n");
        return;
    }

    /*
     ** get the file's size and make sure it exists 
     */

    f = fopen(FICL_COUNTED_STRING_GET_POINTER(*counted), "r");
    if (!f)
    {
        ficlVmTextOut(vm, "Unable to open file ");
        ficlVmTextOut(vm, FICL_COUNTED_STRING_GET_POINTER(*counted));
        ficlVmTextOut(vm, "\n");
        ficlVmThrow(vm, FICL_VM_STATUS_QUIT);
    }

    oldSourceId = vm->sourceId;
    vm->sourceId.p = (void *)f;

    /* feed each line to ficlExec */
    while (fgets(buffer, BUFFER_SIZE, f))
    {
        int length = strlen(buffer) - 1;

        line++;
        if (length <= 0)
            continue;

        if (buffer[length] == '\n')
            buffer[length--] = '\0';

        FICL_STRING_SET_POINTER(s, buffer);
        FICL_STRING_SET_LENGTH(s, length + 1);
        result = ficlVmExecuteString(vm, s);
        /* handle "bye" in loaded files. --lch */
        switch (result)
        {
            case FICL_VM_STATUS_OUT_OF_TEXT:
            case FICL_VM_STATUS_USER_EXIT:
                break;

            default:
                vm->sourceId = oldSourceId;
                fclose(f);
                ficlVmThrowError(vm, "Error loading file <%s> line %d", FICL_COUNTED_STRING_GET_POINTER(*counted), line);
                break; 
        }
    }
    /*
     ** Pass an empty line with SOURCE-ID == -1 to flush
     ** any pending REFILLs (as required by FILE wordset)
     */
    vm->sourceId.i = -1;
    FICL_STRING_SET_FROM_CSTRING(s, "");
    ficlVmExecuteString(vm, s);

    vm->sourceId = oldSourceId;
    fclose(f);

    /* handle "bye" in loaded files. --lch */
    if (result == FICL_VM_STATUS_USER_EXIT)
        ficlVmThrow(vm, FICL_VM_STATUS_USER_EXIT);
    return;
}
#endif


/*
 ** Dump a tab delimited file that summarizes the contents of the
 ** dictionary hash table by hashcode...
 */
static void ficlPrimitiveSpewHash(ficlVm *vm)
{
    ficlHash *hash = ficlVmGetDictionary(vm)->forthWordlist;
    ficlWord *word;
    FILE *f;
    unsigned i;
    unsigned hashSize = hash->size;

    if (!ficlVmGetWordToPad(vm))
        ficlVmThrow(vm, FICL_VM_STATUS_OUT_OF_TEXT);

    f = fopen(vm->pad, "w");
    if (!f)
    {
        ficlVmTextOut(vm, "unable to open file\n");
        return;
    }

    for (i = 0; i < hashSize; i++)
    {
        int n = 0;

        word = hash->table[i];
        while (word)
        {
            n++;
            word = word->link;
        }

        fprintf(f, "%d\t%d", i, n);

        word = hash->table[i];
        while (word)
        {
            fprintf(f, "\t%s", word->name);
            word = word->link;
        }

        fprintf(f, "\n");
    }

    fclose(f);
    return;
}

static void ficlPrimitiveBreak(ficlVm *vm)
{
    vm->state = vm->state;
    return;
}

#ifdef ATH
#include <dlfcn.h>

static void athDlOpen(ficlVm * vm) {
    int libLen;
    char *lib;
    void *res;

    libLen = ficlStackPopInteger(vm->dataStack);
    lib = ficlStackPopPointer(vm->dataStack);

    lib[libLen] = '\0';

    res=(void *)dlopen(lib,RTLD_LAZY );

    if(!res) {
        printf("%s\n", dlerror());
    }    

    ficlStackPushPointer( vm->dataStack,res);

    if( res == (void *)NULL ) {
        ficlStackPushInteger(vm->dataStack,-1);
    } else {
        ficlStackPushInteger(vm->dataStack,0);
    }    
}

#define MAX_ARGS 7
// TODO Is this still true ?
// Note the arguments are in reverse order to the C function.
// i.e:
// void func(a,b,c)
//
// Is called by
//
// c b a 0 3 <ptr to func>
//
// argn ... arg0 <res_count> <arg_count> <function ptr>
// 
static void athDlExec(ficlVm * vm) {
    int i;
    int argCount=0;
    int resCount=0;

    void *(*func)();
    void *args[MAX_ARGS];
    void *res;

    func = ficlStackPopPointer(vm->dataStack);
    argCount = ficlStackPopInteger(vm->dataStack);
    resCount = ficlStackPopInteger(vm->dataStack);

    for (i=0; i < argCount;i++) {
        args[i]  = ficlStackPopPointer(vm->dataStack);
    }

    if ( 0 == argCount && 0 == resCount ) {
        (void)(*func)();
    } else {
        if(resCount > 0 ) {
            switch(argCount) {
                case 0:
                    res=(*func)();
                    break;
                case 1:
                    res=(*func)(args[0]);
                    break;
                case 2:
                    res=(*func)(args[1],args[0]);
                    break;
                case 3:
                    res=(*func)(args[2],args[1],args[0]);
                    break;
                case 4:
                    res=(*func)(args[3],args[2],args[1],args[0]);
                    break;
                case 5:
                    res=(*func)(args[4],args[3],args[2],args[1],args[0]);
                    break;
                case 6:
                    res=(*func)(args[5],args[4],args[3],args[2],args[1],args[0]);
                    break;
                case 7:
                    res=(*func)(args[6],args[5],args[4],args[3],args[2],args[1],args[0]);
                    break;
                default:
                    printf("Too many args\n");
                    break;
            }
            ficlStackPushPointer( vm->dataStack,res);
        } else {
            switch(argCount) {
                case 1:
                    (void)(*func)(args[0]);
                    break;
                case 2:
                    (void)(*func)(args[1],args[0]);
                    break;
                case 3:
                    (void)(*func)(args[2],args[1],args[0]);
                    break;
                case 4:
                    (void)(*func)(args[3],args[2],args[1],args[0]);
                    break;
                case 5:
                    (void)(*func)(args[4],args[3],args[2],args[1],args[0]);
                    break;
                default:
                    printf("Too many args\n");
                    break;
            }
        }
    }

}

static void athDlClose(ficlVm * vm) {
    int res; 
    void *h;

    h = ficlStackPopPointer( vm->dataStack);
    res = dlclose(h);
    ficlStackPushInteger(vm->dataStack,res);
}

static void athDlError(ficlVm *vm) {
    perror(dlerror());
}


/*
 *    name len handle -- symbol_ptr
 */
static void athDlSym(ficlVm * vm) {
    //    void *sym;
    void *h;
    char *symbol;
    char *error;
    char flag=0;

    int symbolLen = 0;

    h = ficlStackPopPointer( vm->dataStack);
    symbolLen = ficlStackPopInteger( vm->dataStack);
    symbol = ficlStackPopPointer(vm->dataStack);
    symbol[symbolLen]=0x00;

    error = dlerror();
    symbol = dlsym(h,symbol);
    error = dlerror();

    if( error != (char *)NULL ) {
        fprintf(stderr,"%s\n",error);
        flag=-1;
    } else {
        flag=0;
    }

    ficlStackPushPointer( vm->dataStack,symbol);
    ficlStackPushInteger( vm->dataStack,flag);

}
//
// Stack: buffer env_name len  --
//
static void athGetenv(ficlVm * vm) {
    char           *ptr;
    char           *env;
    char           *tmp;

    int             len;

    len = ficlStackPopInteger(vm->dataStack);
    env = (char *)ficlStackPopPointer(vm->dataStack);
    ptr = (char *)ficlStackPopPointer(vm->dataStack);

    env[len] = '\0';
    tmp = getenv(env);

    if (tmp) {
        strcpy(ptr, tmp);
        len = strlen(tmp);
    } else {
        len = 0; 
    }    

    ficlStackPushPointer(vm->dataStack, ptr);
    ficlStackPushInteger(vm->dataStack, len);
}

static void athGetErrno(ficlVm * vm) {
    extern int errno;
    ficlStackPushInteger(vm->dataStack, errno);
    errno = 0; 
}

static void athClrErrno(ficlVm * vm) {
    extern int errno;
    errno = 0; 
}

static void athPerror(ficlVm * vm) {
    perror("ficl");
}

static void athGetVM(ficlVm *vm) {

    ficlStackPushPointer(vm->dataStack, vm);
}

static void athSocket(ficlVm * vm) {
    int             sock1;
    sock1 = socket(AF_INET, SOCK_STREAM, 0);
    ficlStackPushInteger(vm->dataStack, sock1);
}

static void athConnect(ficlVm * vm) {
    char           *hostName;
    int             len, port;
    int             tmp;
    int             sock1;
    int             exitStatus = 0; 
    struct sockaddr_in serv_addr;
    //    struct hostent *hp; 
    int rc;

    struct addrinfo *result = NULL;
    struct addrinfo hint;

    char portNumber[8];

    memset(&hint, 0 , sizeof(hint));

    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;

    port = ficlStackPopInteger(vm->dataStack);

    sprintf(portNumber,"%d",port);
    len = ficlStackPopInteger(vm->dataStack);

    hostName = (char *)ficlStackPopPointer(vm->dataStack);
    hostName[len] = '\0';

    rc = getaddrinfo(hostName, portNumber, &hint, &result);

    if( 0 == rc ) {
        sock1 = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if(sock1 < 0) { 
            exitStatus = -1;
        } else {
            tmp = connect(sock1, result->ai_addr, result->ai_addrlen );
            if (tmp < 0) 
                exitStatus = -1;
        }    
    }    

    if (exitStatus == 0) { 
        ficlStackPushInteger(vm->dataStack, sock1);
    }    
    ficlStackPushInteger(vm->dataStack, exitStatus);
}

/* Stack: buffer len socket -- count */

static void athRecv(ficlVm * vm) {
    int             n;
    int             sock2;
    int             len;
    int flag = 0;
    char           *msg;

    sock2 = ficlStackPopInteger(vm->dataStack);
    len = ficlStackPopInteger(vm->dataStack);
    msg =(char *)ficlStackPopPointer(vm->dataStack);
    n = recv(sock2, msg, len, 0);
    ficlStackPushInteger(vm->dataStack, n);
}

static void athSend(ficlVm * vm) {
    char           *buffer;
    int             len;
    int flag=0;
    int             sock2;
    int             status;

    sock2 = ficlStackPopInteger(vm->dataStack);
    len = ficlStackPopInteger(vm->dataStack);
    buffer = (char *)ficlStackPopPointer(vm->dataStack);

    status = send(sock2, buffer, len, 0);
    ficlStackPushInteger(vm->dataStack, status);
    flag = ( status < 0 );
    ficlStackPushInteger(vm->dataStack, flag);

}

static void athGetService(ficlVm *vm) {

    char *serviceName=NULL;
    uint32_t nameLen=0;
    struct servent *serv=NULL;
    uint32_t port=0;

    nameLen = ficlStackPopInteger(vm->dataStack);
    serviceName = (char *)ficlStackPopPointer(vm->dataStack);

    serv = getservbyname(serviceName,NULL);
    if (serv == NULL) {
        port=0;
    } else {
        port=ntohs(serv->s_port);
    }

    ficlStackPushInteger(vm->dataStack, port);
}

static void athClose(ficlVm * vm) {
    int             sock;

    sock = ficlStackPopInteger(vm->dataStack);
    close(sock);
}
// 
// Add an end of line char (0x0a) to a string.
// Space must already exist for this.
//
// Stack : addr len -- addr len+1
//
//
static void athAddCr(ficlVm *vm) {
    char *from;
    int len; 

    len = ficlStackPopInteger(vm->dataStack);
    from = ficlStackPopPointer(vm->dataStack);

    from[len] = (char)0x0a;

    len++;

    ficlStackPushPointer( vm->dataStack,from);
    ficlStackPushInteger(vm->dataStack, len);
}

// 
// Stack : -- hh mm ss
//
static void athGetTime(ficlVm *vm) {
    time_t now = time(NULL);
    struct tm *hms = localtime( &now );

    ficlStackPushPointer( vm->dataStack,hms->tm_hour);
    ficlStackPushPointer( vm->dataStack,hms->tm_min);
    ficlStackPushPointer( vm->dataStack,hms->tm_sec);
}

#endif

void ficlSystemCompileExtras(ficlSystem *system)
{
    ficlDictionary *dictionary = ficlSystemGetDictionary(system);
#ifdef ATH
    ficlDictionarySetPrimitive(dictionary, (char *)"perror", athPerror, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"errno", athGetErrno, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"clr-errno", athClrErrno, FICL_WORD_DEFAULT);

    ficlDictionarySetPrimitive(dictionary, "dlopen", athDlOpen, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "dlclose", athDlClose, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "dlsym", athDlSym, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "dlerror", athDlError, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "dlexec", athDlExec, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"getenv", athGetenv, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"get-vm", athGetVM, FICL_WORD_DEFAULT);

    ficlDictionarySetPrimitive(dictionary, (char *)"time", athGetTime, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"socket", athSocket, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"socket-recv", athRecv, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"socket-send", athSend, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"socket-connect", athConnect, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"socket-service", athGetService, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"socket-close", athClose, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, (char *)"add-cr", athAddCr, FICL_WORD_DEFAULT);

#endif

    ficlDictionarySetPrimitive(dictionary, "break",    ficlPrimitiveBreak,    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "load",     ficlPrimitiveLoad,     FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "spewhash", ficlPrimitiveSpewHash, FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "system",   ficlPrimitiveSystem,   FICL_WORD_DEFAULT);

#ifndef FICL_ANSI
    ficlDictionarySetPrimitive(dictionary, "clock",    ficlPrimitiveClock,    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "strsave",  athStrsave,    FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "strfree",  athStrFree,    FICL_WORD_DEFAULT);
    ficlDictionarySetConstant(dictionary, "clocks/sec", CLOCKS_PER_SEC);
    ficlDictionarySetPrimitive(dictionary, "pwd",      ficlPrimitiveGetCwd,   FICL_WORD_DEFAULT);
    ficlDictionarySetPrimitive(dictionary, "cd",       ficlPrimitiveChDir,    FICL_WORD_DEFAULT);
#endif /* FICL_ANSI */

    return;
}

