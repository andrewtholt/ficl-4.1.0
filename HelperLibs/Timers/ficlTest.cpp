#include "ficlTimer.h"

char *printBool(bool v) {
    if(v) {
        return((char *)" TRUE ");
    } else {
        return((char *)" TRUE ");
    }
}

void fred() {
    printf("============\n");
    printf("Callback run\n");
    printf("============\n");
}

int main() {

    int delay=10;
    int idx=-1;
    bool flag;
    int v=0;

    timerMaster *tst;

    tst = newTimerMaster();

    idx = addTimer(tst, delay);

    flag = startTimer(tst,idx);

    printf("Set callback\n");

    setCallback(tst,idx,fred);

    printf("Start Timer returned:%s \n\n", printBool(flag));

    display(tst);

    updateTimers(tst, 2);

    printf("Updated timers\n");

    display(tst);

    printf("Reset  timer\n");

    flag = resetTimer(tst, idx);

    printf("\tReturned %s\n", printBool(flag));

    display(tst);


    flag = stopTimer(tst,idx);

    printf("Stop Timer returned :%s \n\n", printBool(flag));

    display(tst);

    printf("Restarted timer, and updateTimer\n");
    flag = startTimer(tst,idx);

    for(int i=0;i< 6;i++) {
        updateTimers(tst, 2);

        printf("Next Timer\n");
        v=nextTimer(tst);
        printf("\tReturned %d\n", v);
        display(tst);
    }
    /*

       tst = new ficlTimer(delay);

*/
}

