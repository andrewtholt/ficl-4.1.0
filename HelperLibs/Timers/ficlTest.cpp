#include "ficlTimer.h"

int main() {

    ficlTimer *tst;
    int delay=10;

    tst = new ficlTimer(delay);

    tst->display();
}

