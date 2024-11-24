#pragma once
#include "LinkedQueue.h"
#include "Patient.h"
#include "Car.h"
#include "priQueue.h"

class CancelQueue : public LinkedQueue<Patient*> {
public:

    bool cancelBeforeCarMoves(Patient* P, priQueue<Car*>& OC, priQueue<Car*>& BC);
    bool cancelBeforePickup(Patient* P, priQueue<Car*>& OC, priQueue<Car*>& BC);
    bool checkAndCancelOnCar(Patient* P, priQueue<Car*>& OC, priQueue<Car*>& BC);
};
