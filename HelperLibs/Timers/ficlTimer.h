
#include <newTimer.h>

extern "C" {
#include "ficl.h"
}

class ficlTimer: public myTimer {
    private:
        ficlVm *vm;
        void *xt;
    public:
        ficlTimer(int v);
        void display();
        void ficlSetup(ficlVm *v, ficlWord *x);

};


