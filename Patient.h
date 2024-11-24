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
    int cancelationTime;
    bool isPickedUp;
    bool isCancelled;


public:
    Patient();

    Patient(int pType, int PID, int HID, int distance, int requestTime, int severity);

    Patient(int PID, int HID, int cancelationTime);

    Patient(const Patient &other);

    int getPID() const;

    int getHID() const;

    int getPType() const;

    int getWaitingTime() const;

    int getSeverity() const;

    int getDistance() const;

    int getRequestTime() const;

    int getCancelationTime() const;

    void setDistance(int distance);

    void setPickupTime(int pickupTime);

    void setHID(int HID);

    void setCancelationTime(int cancelationTime);

    void calculateWaitingTime();
};
