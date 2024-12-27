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
    bool isInCheckup;
    int checkupEndTime;
    double failureProbability;

public:
    Car();
    static const int DEFAULT_NORMAL_SPEED = 5;  // Default normal car speed
    static const int DEFAULT_SPECIAL_SPEED = 11; // Default special car speed
    Car(int carType, int speed, int HID);

    Car(const Car& other);
    int getSpeed() const { 
        if (speed <= 0) {
            return (carType == 1) ? DEFAULT_NORMAL_SPEED : DEFAULT_SPECIAL_SPEED;
        }
        return speed;
    }

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
    bool getIsInCheckup() const { return isInCheckup; }
    void setIsInCheckup(bool value) { isInCheckup = value; }
    void setCheckupEndTime(int time) { checkupEndTime = time; }
    int getCheckupEndTime() const { return checkupEndTime; }
    double getFailureProbability() const { return failureProbability; }
    void setFailureProbability(double prob) { failureProbability = prob; }
    void removePatient();
};
