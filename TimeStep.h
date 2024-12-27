#pragma once

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
