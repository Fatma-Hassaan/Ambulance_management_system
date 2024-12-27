#ifndef GLOBALS_H
#define GLOBALS_H

class GlobalTimeStep {
private:
    static int TimeStep;
public:
    GlobalTimeStep() {
        TimeStep = 1;
    }

    static int getTimeStep() {
        return TimeStep;
    }

    static void IncrementTimeStep() {
        TimeStep += 1;
    }
};


#endif // GLOBALS_H