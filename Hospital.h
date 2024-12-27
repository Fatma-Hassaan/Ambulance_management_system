#pragma once
#include "LinkedQueue.h"
#include "priQueue.h"
#include "Patient.h"
#include "Car.h"
#include "TimeStep.h"

class Hospital
{
private:
	int TS=0;					//timeStep			//delete
	int H_ID;
	int total_SC;
	int total_NC;
	int free_SC;
	int free_NC;
	int NC_Speed;
	int SC_Speed;
	int Current_NP_number=0;
	int Current_SP_number=0;
	int Current_EP_number=0;
	LinkedQueue<Patient*> NP;
	LinkedQueue<Patient*> SP;
	priQueue<Patient*> EP;
	LinkedQueue<Car*> NC;
	LinkedQueue<Car*> SC;
	LinkedQueue<Car*> checkupQueue;  // Queue for cars in checkup
    const int CHECKUP_DURATION = 3;
public:
	Hospital(int Hospital_ID, int t_NC, int t_SC, int NC_S, int SC_S);
	void incrementTimeStep();		//delete
	int getHospital_ID();
	int getNumber_FreeNC();
	int getNumber_FreeSC();
	int getNumber_CurrentEP();
	int getNumber_CurrentNP();
	int getNumber_CurrentSP();
	void RecievePatient(Patient* P);
	void RecieveBackCar(Car* bCar);
	Car* AssigningPatient();
    void addToCheckup(Car* car, int currentTime);
    void processCheckups(int currentTime);
    priQueue<Patient*>& getEPQueue() { return EP; }
    LinkedQueue<Patient*>& getSPQueue() { return SP; }
    LinkedQueue<Patient*>& getNPQueue() { return NP; }
	Patient* simulatePatient(int);
	Car* simulateCar(int);

};
