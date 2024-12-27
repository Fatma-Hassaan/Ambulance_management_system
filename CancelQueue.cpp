#include "CancelQueue.h"
#include "Car.h"
#include "Hospital.h"
#include "Patient.h"
#include "TimeStep.h"

bool CancelQueue::cancelBeforeCarMoves(Patient* p, priQueue<Car*>& OC, priQueue<Car*>& BC) {
    Node<Patient*>* temp = getFrontNode();
    Node<Patient*>* prev = nullptr;

    while (temp != nullptr) {
        bool carAssigned = false;
        if (temp->getItem() == p) {
            Car* C;
            int pri;
            // Check OC queue
            priQueue<Car*> tempOC = OC;
            while (!tempOC.isEmpty()) {
                tempOC.dequeue(C, pri);
                if (C->getPatient() == p) {
                    carAssigned = true;
                    break;
                }
            }
            // Check BC queue
            priQueue<Car*> tempBC = BC;
            while (!tempBC.isEmpty()) {
                tempBC.dequeue(C, pri);
                if (C->getPatient() == p) {
                    carAssigned = true;
                    break;
                }
            }
            if (!carAssigned) {
                if (prev == nullptr) {
                    frontPtr = temp->getNext();
                } else {
                    prev->setNext(temp->getNext());
                }
                delete temp;
                return true;
            }
        }
        prev = temp;
        temp = temp->getNext();
    }
    return false;
}

bool CancelQueue::cancelBeforePickup(Patient* P, priQueue<Car*>& OC, priQueue<Car*>& BC) {
    Car* c;
    int pri;
    priQueue<Car*> tempOC = OC;
    
    while (!tempOC.isEmpty()) {
        tempOC.dequeue(c, pri);
        if (c->getPatient() == P) {
            // Check if patient is not in BC
            bool inBC = false;
            priQueue<Car*> tempBC = BC;
            Car* bc;
            
            while (!tempBC.isEmpty()) {
                tempBC.dequeue(bc, pri);
                if (bc->getPatient() == P) {
                    inBC = true;
                    break;
                }
            }
            
            if (!inBC) {
                OC.dequeue(c, pri);
                BC.enqueue(c, -(c->getFinishTime()));
                dequeue(P);
                return true;
            }
        }
    }
    return false;
}

bool CancelQueue::CancelOnCar(Patient* P, priQueue<Car*>& OC, priQueue<Car*>& BC) {
    Car* bc;
    int pri;
    priQueue<Car*> tempBC = BC;
    
    while (!tempBC.isEmpty()) {
        tempBC.dequeue(bc, pri);
        if (bc->getPatient() == P) {
            BC.dequeue(bc, pri);
            BC.enqueue(bc, -(bc->getFinishTime()));
            dequeue(P);
            return true;
        }
    }
    return false;
}