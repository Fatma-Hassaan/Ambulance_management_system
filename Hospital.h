#pragma once
#include "LinkedQueue.h"
#include "priQueue.h"
#include "Patient.h"
#include "Car.h"


class Hospital
{
private:
	int H_ID;
	int total_SC;
	int total_NC;
	int free_SC;
	int free_NC;
	int NC_Speed;
	int SC_Speed;
	int Current_NP_number;
	int Current_SP_number;
	int Current_EP_number;
	LinkedQueue<Patient*> NP;
	LinkedQueue<Patient*> SP;
	priQueue<Patient*> EP;
	LinkedQueue<Car*> NC;
	LinkedQueue<Car*> SC;
public:
	Hospital(int Hospital_ID, int t_NC, int t_SC, int NC_S, int SC_S);
	int getHospital_ID();
	int getNumber_FreeNC();
	int getNumber_FreeSC();
	void RecievePatient(Patient* P);
	Car* AssigningPatient();
};
