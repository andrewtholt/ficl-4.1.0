
#include "ficlTimer.h"

extern "C" {
    struct timerMaster *newTimerMaster() {
        struct timerMaster *boss;

        boss = new timerMaster();

        return boss;
    }

    int addTimer(struct timerMaster *boss, int interval) {
        int idx=-1;

        idx = boss->addTimer(interval);

        return idx;
    }

    bool startTimer(struct timerMaster *boss, int idx) {
        bool flag=true;

        flag=boss->startTimer(idx);

        return flag;
    }

    bool stopTimer(struct timerMaster *boss, int idx) {
        bool flag=true;

        flag=boss->stopTimer(idx);

        return flag;
    }

    bool resetTimer(struct timerMaster *boss, int idx) {
        bool flag=true;

        flag=boss->resetTimer(idx);

        return flag;
    }

    int nextTimer(struct timerMaster *boss) {
        int n;

        n = boss->nextTimer();
        return n;
    }

    void updateTimers(struct timerMaster *boss, int interval) {
        boss->updateTimers(interval);
    }

    void setCallback(struct timerMaster *boss, int idx, void (*callback)()) {
        boss->setCallback(idx, callback);
    }

    void display(struct timerMaster *boss) {

        boss->display();
    }
}

