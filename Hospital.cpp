#include "Hospital.h"

Hospital::Hospital(int Hospital_ID, int t_NC, int t_SC, int NC_S, int SC_S) : 
	H_ID(Hospital_ID), total_NC(t_NC), total_SC(t_SC), NC_Speed(NC_S), SC_Speed(SC_S) 
	{
	free_NC = total_NC;
	free_SC = total_SC;
	for (int i = 0; i < total_NC; i++) {
		Car* C= new Car(1, NC_Speed, H_ID);
		NC.enqueue(C);
	}
	for (int i = 0; i < total_SC; i++) {
		Car* C = new Car(2, SC_Speed, H_ID);
		SC.enqueue(C);
	}
}

void Hospital::incrementTimeStep() //delete
{
	TS++;
}

int Hospital:: getHospital_ID() {
	return H_ID;
}

int Hospital::getNumber_CurrentNP() {
	return Current_NP_number;
}

int Hospital::getNumber_CurrentSP() {
	return Current_SP_number;
}

int Hospital::getNumber_CurrentEP() {
	return Current_EP_number;
}

int Hospital:: getNumber_FreeNC() {
	return free_NC;
}

int Hospital:: getNumber_FreeSC() {
	return free_SC;
}

void Hospital:: RecievePatient(Patient* P) {
	switch (P->getPType()) {
	case 1: 
		NP.enqueue(P);
		Current_NP_number++;
		break;
	case 2:
		SP.enqueue(P);
		Current_SP_number++;
		break;
	case 3:
		EP.enqueue(P, P->getSeverity());
		Current_EP_number++;
		break;
	}
}

void Hospital::RecieveBackCar(Car* bCar)
{
	int bCarType = bCar->getCarType();	//check CarType 
	switch (bCarType)
	{
	case 1:			//NormalCar
		NC.enqueue(bCar);
		free_NC++;
		break;
	case 2:			//SpecialCar
		SC.enqueue(bCar);
		free_SC++;
		break;
	}

	bCar->setStatus(1);
}

Car* Hospital::AssigningPatient() {
	Car* C = nullptr;
	Patient* x = nullptr;
	if (Current_EP_number > 0) {
		int priority;
		if (free_NC > 0) {
			NC.dequeue(C);
			EP.dequeue(x, priority);
			C->setPatient(x);
			Current_EP_number--;
			free_NC--;
			return C;
		}
		if (free_SC > 0) {
			NC.dequeue(C);
			EP.dequeue(x, priority);
			C->setPatient(x);
			Current_EP_number--;
			free_SC--;
			return C;
		}
	}
	if (Current_NP_number > 0 && free_NC > 0) {
		NP.dequeue(x);
		NC.dequeue(C);
		C->setPatient(x);
		Current_NP_number--;
		free_NC--;
		return C;
	}
	if (Current_SP_number > 0 && free_SC > 0) {
		SP.dequeue(x);
		SC.dequeue(C);
		C->setPatient(x);
		Current_SP_number--;
		free_SC--;
		return C;
	}
	return C;
}




Patient* Hospital::simulatePatient(int x) {
	Patient* P=nullptr;
	int pri;
	if (10 <= x && x < 20) {
		if (!SP.isEmpty()) {
			SP.dequeue(P);
			Current_SP_number--;
		}
	}
	if (20 <= x && x< 25) {
		if (!EP.isEmpty()) {
			EP.dequeue(P, pri); 
		Current_EP_number--;
		}
	}
	if (30 <= x && x< 40) {
		if (!NP.isEmpty()) {
			NP.dequeue(P); 
		Current_NP_number--;
		}
	}
	return P;
}

Car* Hospital::simulateCar(int x) {
	Car* C=nullptr;
	if (40 <= x && x< 45) {
		if (!SC.isEmpty()) { SC.dequeue(C); 
		free_SC--;
		}
	}
	if (70 <= x && x< 75) {
		if (!NC.isEmpty()) {
			NC.dequeue(C);
			free_NC--;
		}
	}
	return C;
}


int Hospital::getOCSize() const {
	return OC.getSize();
}

int Hospital::getBCSize() const {
	return BC.getSize();
}

void Hospital:: assignSp(Patient*P, Car*C){
	while (!SP.isEmpty()) {
		if (!SC.isEmpty) {
			Patient* sp;
			Car* sc;
			SC.dequeue(sc);
			SP.dequeue(sp);
			sc->setPatient(sp);
			sc->setStatus(1);
		}
		else {
			break;
		}
	}

}
