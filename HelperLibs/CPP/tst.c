#include "cpp.h"
int main() {

    void *n=NULL;

    char tst[] = "testing 1 2 3";

    struct myVector *p = newVector();

    printf("Vector:Size is %d\n", vectorSize(p));

    vectorAppend(p,n);
    printf("Vector:Size is %d\n", vectorSize(p));

    vectorAppend(p,n);
    vectorAppend(p,n);
    vectorAppend(p,n);

    printf("Vector:Size is %d\n", vectorSize(p));

    vectorErase(p,0);
    printf("Vector:Size is %d\n", vectorSize(p));

    vectorEraseAll(p);
    printf("Vector:Size is %d\n", vectorSize(p));

    struct myMap *m = newMap();
    printf("Map:Size is %d\n", mapSize(m));

    mapAdd(m,"Test", tst);
    printf("Map:Size is %d\n", mapSize(m));

    char *res = mapGet(m,"Test");

    printf("Map:Get entry 'Test', result is >%s<\n", res);

}
