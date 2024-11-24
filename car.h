#include "Patient.h"

class Car {
public:
    int PID;
    int HID;
    Patient* assignedPatient;

    Car(int PID, int HID);
};

