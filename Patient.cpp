#include "Patient.h"

Patient::Patient() : PID(0), requestTime(0), pickupTime(0), HID(0), distance(0), severity(0),
pType(0), waitingTime(0), cancelationTime(0) {}

Patient::Patient(int _pType, int _PID, int _HID, int _distance, int _requestTime, int _severity) :
    pType(_pType), PID(_PID), HID(_HID), distance(_distance), requestTime(_requestTime), severity(_severity),
    pickupTime(0), waitingTime(0), cancelationTime(0) {}

Patient::Patient(int _PID, int _HID, int _cancelationTime) :
    PID(_PID), HID(_HID), requestTime(0), pickupTime(0), distance(0), severity(0),
    pType(0), waitingTime(0), cancelationTime(_cancelationTime) {}

// copy constructor
Patient::Patient(const Patient& other) :
    PID(other.PID), requestTime(other.requestTime), pickupTime(other.pickupTime), HID(other.HID),
    distance(other.distance), severity(other.severity), pType(other.pType), waitingTime(other.waitingTime),
    cancelationTime(other.cancelationTime) {}

int Patient::getPID() const { return PID; }

int Patient::getHID() const { return HID; }

int Patient::getPType() const { return pType; }

int Patient::getWaitingTime() const { return waitingTime; }

int Patient::getSeverity() const { return severity; }

int Patient::getDistance() const { return distance; }

int Patient::getRequestTime() const { return requestTime; }

int Patient::getPickupTime() const { return pickupTime; }

int Patient::getCancelationTime() const { return cancelationTime; } // Getter

void Patient::setDistance(int _distance) { distance = _distance; }

void Patient::setPickupTime(int _pickupTime) { pickupTime = _pickupTime; }

void Patient::setHID(int _HID) { HID = _HID; }

void Patient::setCancelationTime(int _cancelationTime) { cancelationTime = _cancelationTime; } // Setter

void Patient::calculateWaitingTime() {
    waitingTime = pickupTime-requestTime;
}

// In Organizer.cpp
void Organizer::checkForCarFailures() {
    Car* C;
    int pri;
    if (!OC.isEmpty()) {
        OC.peek(C, pri);
        // Generate random number between 0 and 1
        double randNum = (double)rand() / RAND_MAX;
        
        if (randNum < C->getFailureProbability()) {
            handleCarFailure(C);
        }
    }
}

void Organizer::handleCarFailure(Car* car) {
    // Remove from out cars
    Car* C;
    int pri;
    OC.dequeue(C, pri);
    numOfOC--;
    
    // Put patient back at front of queue
    Patient* failedPatient = C->getPatient();
    if (failedPatient) {
        int hospitalId = failedPatient->getHID() - 1;
        Hospital* hospital = HospitalsList[hospitalId];
        
        // Put patient at front of appropriate queue based on type
        switch(failedPatient->getPType()) {
            case 1: // NP
                hospital->getNPQueue().enqueueFront(failedPatient);
                break;
            case 2: // SP  
                hospital->getSPQueue().enqueueFront(failedPatient);
                break;
            case 3: // EP
                hospital->getEPQueue().enqueueFront(failedPatient, failedPatient->getSeverity());
                break;
        }
        
        C->removePatient();
    }
    
    // Send car to checkup
    int hospitalId = C->getHID() - 1;
    HospitalsList[hospitalId]->addToCheckup(C, timeStep);
}