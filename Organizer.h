#pragma once
#include "LinkedQueue.h"
#include "priQueue.h"
#include "Patient.h"
#include "Car.h"
#include "Hospital.h"
#include <string>
using namespace std;

class Organizer
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
	LinkedQueue<Patient*> AR;	//All Requests
	LinkedQueue<Patient*> FP;	//Finished Patients
	LinkedQueue<Patient*> CR;	//Cancelled Requests
	priQueue<Car*> OC;			//Out Cars
	priQueue<Car*> BC;			//Back Cars
public:
	Organizer(fstream& file, string fileName);
    void incrementTimeStep_and_Execute();
    void AddingPatients();
    void HospitalsAssigningPatients();
	void OutCars();
	void BackCars();
	void EP_Redistribution(Patient* P);
    ~Organizer();
};
