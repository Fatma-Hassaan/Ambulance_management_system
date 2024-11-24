#include "CancelQueue.h"

bool CancelQueue::cancelBeforeCarMoves(Patient* P, priQueue<Car*>& OC, priQueue<Car*>& BC) {

    if (this->remove(P)) {

        return true;
    }
    return false;
}

bool CancelQueue::cancelBeforePickup(Patient* P, priQueue<Car*>& OC, priQueue<Car*>& BC) {
    Car* assignedCar = nullptr;
    priQueue<Car*> tempQueue;


    while (!OC.isEmpty()) {
        int temp;
        OC.dequeue(assignedCar,temp);
        if (assignedCar->getPatient() == P) {
            assignedCar->removePatient();
            BC.enqueue(assignedCar, assignedCar->getFinishTime());
            return true;
        }
        else {
            tempQueue.enqueue(assignedCar, assignedCar->getPickupTime());
        }
    }

    while (!tempQueue.isEmpty()) {
        Car* tempCar;
        int temp;
        tempQueue.dequeue(tempCar,temp);
        OC.enqueue(tempCar, tempCar->getPickupTime());
    }

    return false;
}

bool CancelQueue::checkAndCancelOnCar(Patient* P, priQueue<Car*>& OC, priQueue<Car*>& BC) {
    Car* assignedCar = nullptr;

    while (!OC.isEmpty()) {
        int temp;
        OC.dequeue(assignedCar, temp);
        if (assignedCar->getPatient() == P) {
            OC.enqueue(assignedCar, assignedCar->getPickupTime());
            return true;
        }
        OC.enqueue(assignedCar, assignedCar->getPickupTime());
    }

    return false;
}