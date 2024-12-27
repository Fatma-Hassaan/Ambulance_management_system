#pragma once
#include "TimeStep.h"


class Patient {
private:
    int PID;
    int requestTime;
    int pickupTime;
    int HID;
    int distance;
    int severity;
    int pType;
    int waitingTime;
    int cancelationTime;

public:
    Patient();

    Patient(int pType, int PID, int HID, int distance, int requestTime, int severity);

    Patient(int PID, int HID, int cancelationTime);

    Patient(const Patient& other);

    int getPID() const;

    int getHID() const;

    int getPType() const;

    int getWaitingTime() const;

    int getSeverity() const;

    int getDistance() const;

    int getRequestTime() const;

    int getCancelationTime() const;

    int getPickupTime() const;

    void setDistance(int distance);

    void setPickupTime(int pickupTime);

    void setHID(int HID);

    void setCancelationTime(int cancelationTime);

    void calculateWaitingTime();
};
