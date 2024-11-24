#include "Patient.h"

Patient::Patient() : PID(0), requestTime(0), pickupTimes(0), HID(0), distance(0), severity(0),
pType(0), waitingTime(0), cancelationTime(0) {}

Patient::Patient(int _pType, int _PID, int _HID, int _distance, int _requestTime, int _severity) :
    pType(_pType), PID(_PID), HID(_HID), distance(_distance), requestTime(_requestTime), severity(_severity),
    pickupTimes(0), waitingTime(0), cancelationTime(0) {}

Patient::Patient(int _PID, int _HID, int _cancelationTime) :
    PID(_PID), HID(_HID), requestTime(0), pickupTimes(0), distance(0), severity(0),
    pType(0), waitingTime(0), cancelationTime(_cancelationTime) {}

// copy constructor
Patient::Patient(const Patient& other) :
    PID(other.PID), requestTime(other.requestTime), pickupTimes(other.pickupTimes), HID(other.HID),
    distance(other.distance), severity(other.severity), pType(other.pType), waitingTime(other.waitingTime),
    cancelationTime(other.cancelationTime) {}

int Patient::getPID() const { return PID; }

int Patient::getHID() const { return HID; }

int Patient::getPType() const { return pType; }

int Patient::getWaitingTime() const { return waitingTime; }

int Patient::getSeverity() const { return severity; }

int Patient::getDistance() const { return distance; }

int Patient::getRequestTime() const { return requestTime; }

int Patient::getCancelationTime() const { return cancelationTime; } // Getter

void Patient::setDistance(int _distance) { distance = _distance; }

void Patient::setPickupTime(int _pickupTime) { pickupTimes = _pickupTime; }

void Patient::setHID(int _HID) { HID = _HID; }

void Patient::setCancelationTime(int _cancelationTime) { cancelationTime = _cancelationTime; } // Setter

void Patient::calculateWaitingTime() {
    waitingTime = (severity * 100) / (distance + 1);
}
