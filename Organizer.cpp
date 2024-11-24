#include "Organizer.h"
#include <string.h>
#include <fstream>	//for file reading
#include <iostream>	//for file reading


Organizer::Organizer(fstream & file, string fileName)
{
    file.open(fileName, ios::in);
    if (file.fail()) { cout << "File Open Failure!!"; }	//Check that the File opens correctly

    file >> numOfHospitals;		//Assign number of Hospitals

    DistanceMatrix = new int* [numOfHospitals];
    for (int i = 0; i < numOfHospitals; i++)		//Filling DistanceMatrix
    {
        DistanceMatrix[i] = new int[numOfHospitals];
        for (int j = 0; j < numOfHospitals; j++)
        {
            file >> DistanceMatrix[i][j];
        }
    }

    // Assigning Car speeds
    file >> SCS;
    file >> NCS;

    HospitalsList = new Hospital * [numOfHospitals];
    for (int i = 0; i < numOfHospitals; i++)		//Initializig each Hospital with its data
    {
        int tSC, tNC;
        file >> tSC; file >> tNC;
            HospitalsList[i] = new Hospital(i + 1, tNC, tSC, NCS, SCS);
    }

    file >> numOfAR;		//Assign number of all requests

    for (int i = 0; i < numOfAR; i++)		//Initializig each Patient with its data
    {
        int iPT;
        int RT;
        int Pid;
        int Hid;
        int dis;
        int sev;
        string sPT;
        file >> sPT;
        if (sPT == "NP") {
            iPT = 1;
            file >> RT;
            file >> Pid;
            file >> Hid;
            file >> dis;
            sev = 0;
        }
        if (sPT == "SP") {
            iPT = 2;
            file >> RT;
            file >> Pid;
            file >> Hid;
            file >> dis;
            sev = 0;
        }
        if (sPT == "EP") {
            iPT = 3;
            file >> RT;
            file >> Pid;
            file >> Hid;
            file >> dis;
            file >> sev;
        }

        Patient* ptr = new Patient(iPT, Pid, Hid, dis, RT, sev);

        AR.enqueue(ptr);		//Enqueue Patients in AllPatients List
    }

    file >> numOfCR;		//Assign number of Cancellation Requests

    for (int i = 0; i < numOfCR; i++)		//Initializing Patients with Cancellation Requests
    {
        int CT; file >> CT;
        int Pid; file >> Pid;
        int Hid; file >> Hid;
        Patient* ptr = new Patient(Pid, Hid, CT);

        CR.enqueue(ptr);
    }

    file.close();
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
        int PatientType = P->getPType();
        if (PatientType == 3 && ((HospitalsList[HospitalID - 1]->getNumber_FreeNC() + HospitalsList[HospitalID - 1]->getNumber_FreeSC()) <= HospitalsList[HospitalID - 1]->getNumber_CurrentEP())) {
            EP_Redistribution(P);
        }
        else {
            HospitalsList[HospitalID - 1]->RecievePatient(P);
        }
        AR.peek(P);
    }
}

void Organizer::OutCars()
{
    Car* C;
    int pri;
    OC.peek(C, pri);
    while (pri == -timeStep)
    {
        OC.dequeue(C, pri);
        BC.enqueue(C, -(C->getFinishTime()));
        C->setStatus(3);
        numOfOC--;
        numOfBC++;

        OC.peek(C,pri);
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

void Organizer::BackCars()
{
    Car* C;
    int pri;
    BC.peek(C, pri);
    while (C->getFinishTime() == timeStep)			// when timestep reaches the Finishtime
    {
        BC.dequeue(C, pri);		//Dequeue Car from BackCars List

        Patient* ptr = C->getPatient();
        FP.enqueue(ptr);					//Enqueue Patient inside FinishedPatients
        C->removePatient();					//List and remove it from the Car

        C->setStatus(1);					//Set Car status back to ready and 
        numOfBC--;						// decrement number of BackCars by 1

        int Hid = C->getHID();
        HospitalsList[Hid - 1]->RecieveBackCar(C);		//Send the Car back to its Hospital

        BC.peek(C, pri); 
    }
}



void Organizer::EP_Redistribution(Patient* P) {
    int currentHospitalID = P->getHID();
    int patientType = P->getPType();
    bool redistributed = false;
    for (int i = 0; i < numOfHospitals; i++) {
        if (i + 1 == currentHospitalID) continue;

        Hospital* targetHospital = HospitalsList[i];
        if (patientType == 1 && targetHospital->getNumber_FreeNC() > 0) {

            targetHospital->RecievePatient(P);
            P->setHID(i + 1);
            redistributed = true;
            break;
        }
        else if (patientType == 2 && targetHospital->getNumber_FreeSC() > 0) {

            targetHospital->RecievePatient(P);
            P->setHID(i + 1);
            redistributed = true;
            break;
        }
    }

    if (!redistributed) {

        AR.enqueue(P);
    }
}