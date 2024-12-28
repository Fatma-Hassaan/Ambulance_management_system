#include "Organizer.h"
#include <string.h>
#include <fstream>	//for file reading
#include <iostream>	//for file reading
#include <time.h>
#include <cstdlib>


Organizer::Organizer(fstream & file, string fileName)
{
    numOfOC = 0;
    numOfBC = 0;
    numOfFP = 0;
    numOfReDisEP = 0;
    timeStep = 0;
    file.open(fileName, ios::in);
    if (file.fail()) { cout << "File Open Failure!!"; }	

    file >> numOfHospitals;		

    DistanceMatrix = new int* [numOfHospitals];
    for (int i = 0; i < numOfHospitals; i++)	
    {
        DistanceMatrix[i] = new int[numOfHospitals];
        for (int j = 0; j < numOfHospitals; j++)
        {
            file >> DistanceMatrix[i][j];
        }
    }


    file >> SCS;
    file >> NCS;

    HospitalsList = new Hospital * [numOfHospitals];
    for (int i = 0; i < numOfHospitals; i++)	
    {
        int tSC, tNC;
        file >> tSC; file >> tNC;
        HospitalsList[i] = new Hospital(i + 1, tNC, tSC, NCS, SCS);
    }

    file >> numOfAR;	
    for (int i = 0; i < numOfAR; i++)		
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

        AR.enqueue(ptr);		
    }

    file >> numOfCR;		

    for (int i = 0; i < numOfCR; i++)	
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
    bool simulationComplete = false;
    
    while (!simulationComplete) {
        timeStep++;
        
        AddingPatients();        
        ProcessCancellations();  
        HandleOutCars();         
        HandleBackCars();        
        
        // 2. Update hospitals
        for (int i = 0; i < numOfHospitals; i++) {
            // Try to assign available cars to waiting patients
            Car* assignedCar = HospitalsList[i]->AssigningPatient();
            if (assignedCar) {
                int pickupTime = timeStep + (assignedCar->getPatient()->getDistance() / assignedCar->getSpeed());
                assignedCar->setPickupTime(pickupTime);
                OC.enqueue(assignedCar, -pickupTime); // negative for priority queue ordering
                numOfOC++;
            }
        }

        PrintTimeStepStatus();
        
        if (IsSimulationComplete()) {
            simulationComplete = true;
            GenerateOutputFile();
        }
    }
}

bool Organizer::IsSimulationComplete() {
    if (timeStep < 5) return false;
    
    for (int i = 0; i < numOfHospitals; i++) {
        if (HospitalsList[i]->getNumber_CurrentEP() > 0 ||
            HospitalsList[i]->getNumber_CurrentNP() > 0 ||
            HospitalsList[i]->getNumber_CurrentSP() > 0) {
            return false;
        }
    }
    
    return (numOfOC == 0 && numOfBC == 0);
}

void Organizer::PrintTimeStepStatus() {
    cout << "\nTimestep: " << timeStep << endl;
    cout << "Out Cars: " << numOfOC << " Back Cars: " << numOfBC << endl;
    cout << "Finished Patients: " << numOfFP << endl;
    
    for (int i = 0; i < numOfHospitals; i++) {
        cout << "\nHospital " << i+1 << " Status:" << endl;
        cout << "EP: " << HospitalsList[i]->getNumber_CurrentEP();
        cout << " NP: " << HospitalsList[i]->getNumber_CurrentNP();
        cout << " SP: " << HospitalsList[i]->getNumber_CurrentSP() << endl;
    }
}


void Organizer::AddingPatients() {
    Patient* P;
    if (!AR.isEmpty()){
        AR.peek(P);
        while (P->getRequestTime() == timeStep) {
            AR.dequeue(P);
            int HospitalID = P->getHID();
            int PatientType = P->getPType();
            HospitalsList[HospitalID - 1]->RecievePatient(P);
/*
            if (PatientType == 3 && ((HospitalsList[HospitalID - 1]->getNumber_FreeNC() + HospitalsList[HospitalID - 1]->getNumber_FreeSC()) <= HospitalsList[HospitalID - 1]->getNumber_CurrentEP())) {
                EP_Redistribution(P);
            }
            else {
                HospitalsList[HospitalID - 1]->RecievePatient(P);
            }
            */
            if (!AR.isEmpty()) AR.peek(P);
            else break;
        }
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
    numOfReDisEP++;
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
    
    cout << timeStep;
}

void Organizer::ProcessCancellations() {
    Patient* P;
    if (!CR.isEmpty()) {
        CR.peek(P);
        while (P->getCancelationTime() == timeStep) {
            CR.dequeue(P);

            // Try to cancel from waiting queues in hospitals
            int hospitalId = P->getHID() - 1;
            Hospital* hospital = HospitalsList[hospitalId];
            
            // Try to cancel from out cars, using inherited methods
            if (!CancelQueue::cancelBeforePickup(P, OC, BC)) {
                // Check if patient is in a car that's already moving
                CancelQueue::checkAndCancelOnCar(P, OC, BC);
            }

            if (!CR.isEmpty()) {
                CR.peek(P);
            } else {
                break;
            }
        }
    }
}

void Organizer::HandleOutCars() {
    Car* C;
    int pri;
    if (!OC.isEmpty()) {
        OC.peek(C, pri);
        while (pri == -timeStep) {
            OC.dequeue(C, pri);
            BC.enqueue(C, -(C->getFinishTime()));
            C->setStatus(3);
            numOfOC--;
            numOfBC++;

            if (!OC.isEmpty()) {
                OC.peek(C, pri);
            } else {
                break;
            }
        }
    }
}

void Organizer::HandleBackCars() {
    Car* C;
    int pri;
    if (!BC.isEmpty()) {
        BC.peek(C, pri);
        while (C->getFinishTime() == timeStep) {
            BC.dequeue(C, pri);
            
            Patient* ptr = C->getPatient();
            FP.enqueue(ptr);
            C->removePatient();
            
            C->setStatus(1);
            numOfBC--;
            
            int Hid = C->getHID();
            HospitalsList[Hid - 1]->RecieveBackCar(C);
            
            if (!BC.isEmpty()) {
                BC.peek(C, pri);
            } else {
                break;
            }
        }
    }
}

void Organizer::GenerateOutputFile() {
    ofstream outFile("simulation_output.txt");
    
    if (!outFile.is_open()) {
        cout << "Error: Could not create output file" << endl;
        return;
    }

    outFile << "Simulation Ended at TimeStep " << timeStep << endl;
    outFile << "\nStatistics:" << endl;
    outFile << "Total Patients: " << numOfAR << endl;
    outFile << "Total Finished Patients: " << numOfFP << endl;
    outFile << "Average Wait Time: " << calculateAverageWaitTime() << endl;
    outFile << "Auto-Promoted Patients: " << numOfReDisEP << endl;

    // Print hospital statistics
    for (int i = 0; i < numOfHospitals; i++) {
        outFile << "\nHospital " << (i+1) << " Statistics:" << endl;
        outFile << "Number of available Normal Cars: " << HospitalsList[i]->getNumber_FreeNC() << endl;
        outFile << "Number of available Special Cars: " << HospitalsList[i]->getNumber_FreeSC() << endl;
    }

    outFile.close();
}

double Organizer::calculateAverageWaitTime() {
    if (numOfFP == 0) return 0;
    
    double totalWaitTime = 0;
    LinkedQueue<Patient*> tempQueue = FP;  // Create a temporary copy
    Patient* p;
    
    while (!tempQueue.isEmpty()) {
        tempQueue.dequeue(p);
        totalWaitTime += p->getWaitingTime();
    }
    
    return totalWaitTime / numOfFP;
}
void Organizer::printalllist() {
 // 1. AR
    cout << "Active Requests" << endl; 
    LinkedQueue<Patient>* tempAR = AR; 
    Patient* P; 
    while (!AR.isEmpty) {
        tempAR.dequeue(P);
        cout << "Patient" << P->getPID << "Hospital" << P->getHID <<"Request time"<<P->getRequestTime() << endl;
    }


    cout << "cancel requests:" << endl;
    LinkedQueue<Patient>* tempCR = CR;
    Patient* CP; 
    while (!CR.isEmpty) {
        tempCR.dequeue(CP);
        cout << "Patient" << CP->getPID()<< "cancel time" << CP->getCancelationTime() << endl;
    }


    cout << "Out Cars:" << endl;
    priQueue<Car>* tempOC = OC; 
    Car* c; 
    while (!OC.isEmpty) {
        OC.dequeue(c);
        cout << "Car:" << c->getID() << "Patient:"<<c->getPatient()->getPID() << endl;

    }
    cout << "Back cars:" << endl;
    priQueue<Car*>tempBC = BC; 
    Car* bc; 
    while (!BC.isEmpty) {
        BC.dequeue(bc);
        cout << "Car:" << bc->getID() << "Patient:" << bc->getPatient()->getPID() <<"packup time:" <<bc->getPickupTime() << endl;

    }
    cout << "Finished Patients:" << endl;
    LinkedQueue<Patient*> tempFP = FP; 
    Patient* fp;
    while (!FP.isEmpty) {
        FP.dequeue(fp);
        cout << "patient" << fp->getPID << "waiting time" << fp->calculateWaitingTime() << endl;
    }

}
