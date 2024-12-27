#pragma once
#include "LinkedQueue.h"
#include "priQueue.h"
#include "Patient.h"
#include "Car.h"
#include "Hospital.h"
#include <string>
#include "TimeStep.h"
#include "CancelQueue.h"

using namespace std;

class Organizer : public CancelQueue
{
private:
    int timeStep=0;
	int numOfHospitals;
	Hospital** HospitalsList;
	int** DistanceMatrix;
    int NCS;                    //Normal Car Speed
    int SCS;                    //Special Car Speed
    int numOfAR;                //number of All Requests
	int numOfReDisEP;			//number of Redistributed Emergency Patients
    int numOfCR;                //number of Cancelled Requests
	int numOfOC;				//number of Out Cars
	int numOfBC;				//number of Back Cars
	int numOfFP;				//number of Finished Patients
	LinkedQueue<Patient*> AR;	//All Requests
	LinkedQueue<Patient*> FP;	//Finished Patients
	LinkedQueue<Patient*> CR;	//Cancelled Requests
	priQueue<Car*> OC;			//Out Cars
	priQueue<Car*> BC;			//Back Cars
	double defaultFailureProb;  
    void checkForCarFailures();
    void handleCarFailure(Car* car);
	bool silentMode;
    void GenerateOutputFile();	
	double calculateAverageWaitTime();	
public:
	Organizer(fstream& file, string fileName);
	Organizer(fstream& file, string fileName, bool silent = false)
	: silentMode(silent) { /* existing constructor code */ }
    void incrementTimeStep_and_Execute();
	bool IsSimulationComplete();
	void PrintTimeStepStatus();
    void AddingPatients();
    void HospitalsAssigningPatients();
	void OutCars();
	void BackCars();
	void EP_Redistribution(Patient* P);
	void ProcessCancellations();
    void HandleOutCars();  
    void HandleBackCars();
    ~Organizer();
};