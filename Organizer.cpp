#include "Organizer.h"
#include <string.h>
#include <fstream>	//for file reading


Organizer::Organizer(fstream& file, string fileName)
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
	
    HospitalsList = new Hospital* [numOfHospitals];
    for (int i = 0; i < numOfHospitals; i++)		//Initializig each Hospital with its data
    {
	int tSC, tNC;
	file >> tSC; file >> tNC
	HospitalsList[i] = new Hospital(i+1, tNC, tSC, NCS, SCS);
    }

    file >> numOfAR;		//Assign number of all requests

    for (int i = 0; i < numOfAR; i++)		//Initializig each Patient with its data
    {
	string sPT;
	file >> sPT;
	switch(sPT)
	{ case "NP": 
		int iPT = 1; 
		int RT; file >> RT; 
		int Pid; file >> Pid; 
		int Hid; file >> Hid;
		int dis; file >> dis;
		int sev = 0;
	  case "SP":
		int iPT = 2; 
		int RT; file >> RT; 
		int Pid; file >> Pid; 
		int Hid; file >> Hid;
		int dis; file >> dis;
		int sev = 0;
	  case "EP":
		int iPT = 3; 
		int RT; file >> RT; 
		int Pid; file >> Pid; 
		int Hid; file >> Hid;
		int dis; file >> dis;
		int sev; file >> sev;
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

void Organizer::OutCars(Car* oCar, Patient* p)
{
    int pHID = p.getHID; 
	int pType = p.getPType;
    
    int NCnumber = HospitalsList[pHID - 1].getNumber_FreeNC();
	int SCnumber = HospitalsList[pHID - 1].getNumber_FreeSC();

	while (p.getRequestTime() == timeStep)
	{
		switch (pType)
		{
		case 1:               //NormalPatient
			if (NCnumber > 0)
			{
				HospitalsList[pHID - 1].NC.dequeue(oCar);
				OC.enqueue(oCar, -(oCar.getPickupTime()));
				oCar.setStatus(2);
				numOfOC++;
			}

			break;
		case 2:               //SpecialPatient
			if (SCnumber > 0)
			{
				HospitalsList[pHID - 1].SC.dequeue(oCar);
				OC.enqueue(oCar, -(oCar.getPickupTime()));
				oCar.setStatus(2);
				numOfOC++;
			}

			break;
		case 3:               //EmergencyPatient
			if (NCnumber > 0)
			{
				HospitalsList[pHID - 1].NC.dequeue(oCar);
				OC.enqueue(oCar, -(oCar.getPickupTime()));
				oCar.setStatus(2);
				numOfOC++;
			}
			else if (SCnumber > 0)
			{
				HospitalsList[pHID - 1].SC.dequeue(oCar);
				OC.enqueue(oCar, -(oCar.getPickupTime()));
				oCar.setStatus(2);
				numOfOC++;
			}
			else
			{
				// EP_Redistribution
			}

			break;
		}
	}
}

void Organizer::BackCars()
{
	Car* bCar;
	int pri;
	BC.peek(bCar, pri);
	while (bCar.getFinishTime() == timeStep)			// when timestep reaches the Finishtime
	{
		BC.dequeue(bCar, -(bCar.getFinishTime()));		//Dequeue Car from BackCars List

		Patient* ptr = bCar.getPatient();
		FP.enqueue(ptr);					//Enqueue Patient inside FinishedPatients
		bCar.removePatient();					//List and remove it from the Car
									
		bCar.setStatus(1);					//Set Car status back to ready and 
		numOfBC--;						// decrement number of BackCars by 1

		int Hid = bCar.getHID;
		HospitalsList[Hid - 1].RecieveBackCar(bCar);		//Send the Car back to its Hospital

		BC.peek(bCar, pri); //Priority??
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
