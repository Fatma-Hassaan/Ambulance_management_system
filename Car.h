#pragma once
#include "TimeStep.h"
#include "Patient.h"

class Car {
private:
    int carType;
    int speed;
    int status;
    int busyTime;
    int AT;
    Patient* p;
    int pickupTime;
    int finishTime;
    int HID;

public:
    Car();

    Car(int carType, int speed, int HID);

    Car(const Car& other);

    int getCarType() const;

    int getStatus() const;

    int getBusyTime() const;

    int getAT() const;

    int getPickupTime() const;

    int getFinishTime() const;

    int getHID() const;

    Patient* getPatient() const;

    void setStatus(int status);

    void setAT(int _AT);

    void setPatient(Patient* p);

    void setPickupTime(int pickupTime);

    void setFinishTime(int finishTime);

    void calculateBusyTime();

    void removePatient();
};
