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
    while (!IsSimulationComplete()) {
        timeStep++;
        
        
        AddingPatients();
        ProcessCancellations();
        
        
        if (!OC.isEmpty()) {
            Car* C;
            int pri;
            OC.peek(C, pri);
            double randNum = (double)rand() / RAND_MAX;
            
            if (randNum < C->getFailureProbability()) {
        
                OC.dequeue(C, pri);
                numOfOC--;
                
        
                Patient* failedPatient = C->getPatient();
                if (failedPatient) {
                    int hospitalId = failedPatient->getHID() - 1;
                    Hospital* hospital = HospitalsList[hospitalId];
                    
                    switch(failedPatient->getPType()) {
                        case 1: 
                            hospital->getNPQueue().enqueue(failedPatient);
                            break;
                        case 2:
                            hospital->getSPQueue().enqueue(failedPatient);
                            break;
                        case 3:
                            hospital->getEPQueue().enqueue(failedPatient, failedPatient->getSeverity());
                            break;
                    }
                    C->removePatient();
                }
                
                int hospitalId = C->getHID() - 1;
                HospitalsList[hospitalId]->addToCheckup(C, timeStep);
            }
        }
        
        HandleOutCars();
        HandleBackCars();
        
        for (int i = 0; i < numOfHospitals; i++) {
            HospitalsList[i]->processCheckups(timeStep);
        }
        
        for (int i = 0; i < numOfHospitals; i++) {
            Car* assignedCar = HospitalsList[i]->AssigningPatient();
            if (assignedCar) {
                int pickupTime = timeStep + 
                    (assignedCar->getPatient()->getDistance() / assignedCar->getSpeed());
                assignedCar->setPickupTime(pickupTime);
                OC.enqueue(assignedCar, -pickupTime);
                numOfOC++;
            }
        }
        
        PrintTimeStepStatus();
    }
    GenerateOutputFile();
}

bool Organizer::IsSimulationComplete() {
    if (timeStep < 5) return false;
    
    // Check if there are any remaining patients or active cars
    bool noMorePatients = AR.isEmpty() && CR.isEmpty();
    bool noActiveCars = numOfOC == 0;
    bool allCarsProcessed = numOfBC == 0;
    
    // Check all hospitals are empty
    bool hospitalsEmpty = true;
    for (int i = 0; i < numOfHospitals; i++) {
        if (HospitalsList[i]->getNumber_CurrentEP() > 0 ||
            HospitalsList[i]->getNumber_CurrentNP() > 0 ||
            HospitalsList[i]->getNumber_CurrentSP() > 0) {
            hospitalsEmpty = false;
            break;
        }
    }
    
    return noMorePatients && noActiveCars && allCarsProcessed && hospitalsEmpty;
}

void Organizer::PrintTimeStepStatus() {
    if (silentMode) return;
    
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
    while (C->getFinishTime() == timeStep)		
    {
        BC.dequeue(C, pri);		

        Patient* ptr = C->getPatient();
        FP.enqueue(ptr);		
        C->removePatient();		

        C->setStatus(1);		
        numOfBC--;				

        int Hid = C->getHID();
        HospitalsList[Hid - 1]->RecieveBackCar(C);

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
            int hospitalId = P->getHID() - 1;
            Hospital* hospital = HospitalsList[hospitalId];

            if (!CancelQueue::cancelBeforePickup(P, OC, BC)) 
                CancelQueue::CancelOnCar(P, OC, BC); 
            if (!CR.isEmpty()) 
                CR.peek(P);
             else 
                break;
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
        
        cout << "Processing back car with finish time: " << C->getFinishTime() 
             << " at timestep: " << timeStep << endl;
        
        while (!BC.isEmpty() && C->getFinishTime() == timeStep) {
            BC.dequeue(C, pri);
            
            Patient* ptr = C->getPatient();
            if (ptr) {
                FP.enqueue(ptr);
                numOfFP++; 
            }
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
    ofstream outFile("output.txt");
    if (!outFile) {
        cout << "Error: Could not create output file" << endl;
        return;
    }

    outFile << "TT\t" << timeStep << "\n";
    outFile << "FT\t" << numOfFP << "\n";
    outFile << "WC\t" << calculateAverageWaitTime() << "\n";
    outFile << "PA\t" << numOfReDisEP << "\n\n";

    outFile << "patients Info: ";
    LinkedQueue<Patient*> tempFP = FP; // Create copy to preserve original
    Patient* p;
    while (tempFP.dequeue(p)) {
        outFile << "\nPatient " << p->getPID();
        outFile << "\tAT: " << p->getRequestTime();
        outFile << "\tWT: " << p->getWaitingTime();
        outFile << "\tPT: " << p->getPickupTime();
    }
    outFile << "\n\n";

    outFile << "Hospitals Info:\n";
    for (int i = 0; i < numOfHospitals; i++) {
        Hospital* h = HospitalsList[i];
        outFile << "hospital " << h->getHospital_ID() << ":\n";
        outFile << "\tAvailable Normal Cars: " << h->getNumber_FreeNC() << "\n";
        outFile << "\tAvailable Special Cars: " << h->getNumber_FreeSC() << "\n\n";
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

void Organizer::checkForCarFailures() {
    if (OC.isEmpty()) return;
    
    Car* C;
    int pri;
    OC.peek(C, pri);
    
    // Generate random number between 0 and 1
    double randNum = (double)rand() / RAND_MAX;
    
    if (randNum < C->getFailureProbability()) {
        handleCarFailure(C);
    }
}

void Organizer::handleCarFailure(Car* car) {
    // Remove from out cars
    Car* C;
    int pri;
    OC.dequeue(C, pri);
    numOfOC--;
    
    
    Patient* failedPatient = C->getPatient();
    if (failedPatient) {
        int hospitalId = failedPatient->getHID() - 1;
        Hospital* hospital = HospitalsList[hospitalId];
        
        switch(failedPatient->getPType()) {
            case 1: // NP
                hospital->getNPQueue().enqueue(failedPatient);
                break;
            case 2: // SP  
                hospital->getSPQueue().enqueue(failedPatient);
                break;
            case 3: // EP
                hospital->getEPQueue().enqueue(failedPatient, failedPatient->getSeverity());
                break;
        }
        
        C->removePatient();
    }
    
    // Send car to checkup
    int hospitalId = C->getHID() - 1;
    HospitalsList[hospitalId]->addToCheckup(C, timeStep);
}