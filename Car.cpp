
#include "Car.h"

Car::Car() : carType(0), speed(0), status(1), busyTime(0), AT(0), p(nullptr), pickupTime(0), finishTime(0), HID(0) {}

Car::Car(int _carType, int _speed, int _HID) : carType(_carType), speed(_speed), HID(_HID),
                                               status(1), busyTime(0), AT(0), p(nullptr), pickupTime(0),
                                               finishTime(0) {}

Car::Car(const Car &other) : carType(other.carType), speed(other.speed), status(other.status),
                             busyTime(other.busyTime), AT(other.AT), p(other.p), pickupTime(other.pickupTime),
                             finishTime(other.finishTime), HID(other.HID) {}

int Car::getCarType() const { return carType; }

int Car::getStatus() const { return status; }

int Car::getBusyTime() const { return busyTime; }

int Car::getAT() const { return AT; }

int Car::getPickupTime() const { return pickupTime; }

int Car::getFinishTime() const { return finishTime; }

int Car::getHID() const { return HID; }

Patient *Car::getPatient() const { return p; }

void Car::setStatus(int _status) { status = _status; }

void Car::setPatient(Patient *_p) { p = _p; }

void Car::setPickupTime(int _pickupTime) { pickupTime = _pickupTime; }

void Car::setFinishTime(int _finishTime) { finishTime = _finishTime; }

void Car::calculateBusyTime() {
    busyTime = finishTime - AT;
}

Car::Car() : assignedPatient(nullptr) {}

void Car::assignPatient(Patient* P) {
    assignedPatient = P;
}

void Car::removePatient() {
    assignedPatient = nullptr;
}

Patient* Car::getAssignedPatient() {
    return assignedPatient;
}
=======
#include "Car.h"

Car::Car() : carType(0), speed(0), status(1), busyTime(0), AT(0), p(nullptr), pickupTime(0), finishTime(0), HID(0) {}

Car::Car(int _carType, int _speed, int _HID) : carType(_carType), speed(_speed), HID(_HID),
                                               status(1), busyTime(0), AT(0), p(nullptr), pickupTime(0),
                                               finishTime(0) {}

Car::Car(const Car &other) : carType(other.carType), speed(other.speed), status(other.status),
                             busyTime(other.busyTime), AT(other.AT), p(other.p), pickupTime(other.pickupTime),
                             finishTime(other.finishTime), HID(other.HID) {}

int Car::getCarType() const { return carType; }

int Car::getStatus() const { return status; }

int Car::getBusyTime() const { return busyTime; }

int Car::getAT() const { return AT; }

int Car::getPickupTime() const { return pickupTime; }

int Car::getFinishTime() const { return finishTime; }

int Car::getHID() const { return HID; }

Patient *Car::getPatient() const { return p; }

void Car::setStatus(int _status) { status = _status; }

void Car::setPatient(Patient *_p) { p = _p; }

void Car::setPickupTime(int _pickupTime) { pickupTime = _pickupTime; }

void Car::setFinishTime(int _finishTime) { finishTime = _finishTime; }

void Car::calculateBusyTime() {
    busyTime = finishTime - AT;
}

