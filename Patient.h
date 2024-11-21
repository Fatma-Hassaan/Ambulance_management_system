#pragma once

class Patient {
private:
    int PID;
    int requestTime;
    int pickupTimes;
    int HID;
    int distance;
    int severity;
    int pType;
    int waitingTime;

public:
    Patient();

    Patient(int pType, int PID, int HID, int distance, int requestTime, int severity);

    Patient(const Patient &other);

    int getPID() const;

    int getHID() const;

    int getPType() const;

    int getWaitingTime() const;

    int getSeverity() const;

    int getDistance() const;

    int getRequestTime() const;

    void setDistance(int distance);

    void setPickupTime(int pickupTime);

    void setHID(int HID);

    void calculateWaitingTime();
};
