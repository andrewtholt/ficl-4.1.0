
#include "ficlTimer.h"

ficlTimer::ficlTimer(int interval) {
//   myTimer(interval);
    myTimer::set(interval);
}

void ficlTimer::display() {
    myTimer::display();
}

void ficlTimer::ficlSetup(ficlVm *v, ficlWord *x) {
    vm=v;
    xt=x;

    setCallback(NULL);
}

struct ficlTimer *newFiclTimer(int interval) {
    struct ficlTimer *res;

    res = new ficlTimer( interval );

    return res;
}

void ficlSetup(struct ficlTimer *inst, ficlVm *v, ficlWord *x) {

    inst->ficlSetup(v, x);
}

void display(struct ficlTimer *inst) {
    inst->display();
}

