#include "Organizer.h"
#include <string.h>


Organizer::Organizer(string fileName) {
    int numOfHospitals;

    DistanceMatrix = new int* [numOfHospitals];
    for (int i = 0; i < numOfHospitals; i++) {
        DistanceMatrix[i] = new int[numOfHospitals];
    }
    //NCS,SCS
    //Filling DistanceMatrix

    HospitalsList = new Hospital * [numOfHospitals];
    for (int i = 0; i < numOfHospitals; i++) {
        //HospitalsList[i] = new Hospital(The following line);
        //Hospital(Hospital ID which starts with 1, number Of NC in the hospitaL, number Of SC in the hospitaL, Normal Car Speed, Special Car Speed);
    }

    //numOfAR
    for (int i = 0; i < numOfAR; i++) {
        //make the patients using the patients constructor the Naqeeb made (The following line)
        //Patient(int pType, int PID, int HID, int distance, int requestTime, int severity);
        //Add the parients to the all requests queue
    }

    //numOfCR
    for (int i = 0; i < numOfCR; i++) {
        //make other patients that contain only the patient ID, Hospital ID, and the cancellation timestep. Use the constructor (The following line)
        //Patient(int PID, int HID, CancellationTime);
        //Add the parients to the cancellation requests queue
    }

}

void Organizer::incrementTimeStep_and_Execute() {
    timeStep++;
    for (int i = 0; i < numOfHospitals; i++) {
        HospitalsList[i - 1]->incrementTimeStep();
    }
    AddingPatients();
    HospitalsAssigningPatients();
}


void Organizer::AddingPatients() {
    Patient* P;
    AR.peek(P);
    while (P->getRequestTime() == timeStep) {
        AR.dequeue(P);
        int HospitalID = P->getHID();
        int PatientType = P->PType();
        if (PatientType == 3 && ((HospitalsList[HospitalID - 1]->getNumber_FreeNC() + HospitalsList[HospitalID - 1]->getNumber_FreeSC()) <= HospitalsList[HospitalID - 1]->getNumber_CurrentEP())) {
            EP_Redistribution(P);
        }
        else {
            HospitalsList[HospitalID - 1]->RecievePatient(P);
        }
        AR.peek(P);
    }
}

void Organizer::HospitalsAssigningPatients() {
    for (int i = 0; i < numOfHospitals; i++) {
        bool canAssignPatients = true;
        while (canAssignPatients) {
            Car* C = HospitalsList[i - 1]->AssigningPatient();
            if (C == nullptr) break;
            int PUT = C->getPickupTime();
            OC.enqueue(C, -PUT);
            if ((HospitalsList[i - 1]->getNumber_FreeNC() == 0) && (HospitalsList[i - 1]->getNumber_FreeSC() == 0)) {
                canAssignPatients = false;
            }
        }

    }
}

Organizer::~Organizer() {
    for (int i = 0; i < numOfHospitals; i++) {
        delete[] DistanceMatrix[i];
    }
    delete[] DistanceMatrix;
    DistanceMatrix = nullptr;


    for (int i = 0; i < numOfHospitals; i++) {
        delete HospitalsList[i];
    }
    delete[] HospitalsList;
    HospitalsList = nullptr;

}
