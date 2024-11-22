#include "Hospital.h"

Hospital::Hospital(int Hospital_ID, int t_NC, int t_SC, int NC_S, int SC_S) : H_ID(Hospital_ID), total_NC(t_NC), total_SC(t_SC), NC_Speed(NC_S), SC_Speed(SC_S) {
	free_NC = total_NC;
	free_SC = total_SC;
	for (int i = 0; i < total_NC; i++) {
		Car* C(1, NC_Speed, H_ID);
		NC.enqueue(C);
	}
	for (int i = 0; i < total_SC; i++) {
		Car* C = (2, SC_Speed, H_ID);
		SC.enqueue(C);
	}
}

int Hospital:: getHospital_ID() {
	return H_ID;
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
	int patient_Type = P->getP_Type();
	if (patient_Type == 1)
		NP.enqueue(P);
	if (patient_Type == 2)
		SP.enqueue(P);
	if (patient_Type == 3)
		EP.enqueue(P);
}

Car* Hospital::AssigningPatient() {
	Car* C = nullptr;
	Patient* x = nullptr;
	if (Current_EP_number>0) {
		int priority;
		if (free_NC > 0) {
			NC.dequeue(C);
			EP.dequeue(x, priority);
			C->AddPatient(x);
			return C;
		}
		if (free_SC > 0) {
			NC.dequeue(C);
			EP.dequeue(x, priority);
			C->AddPatient(x);
			return C;
		}
	}
	if (Current_NP_number > 0 && free_NC > 0) {
		NP.dequeue(x);
		NC.dequeue(C);
		C->AddPatient(x);
		return C;
	}
	if (Current_SP_number > 0 && free_SC > 0) {
		SP.dequeue(x);
		SC.dequeue(C);
		C->AddPatient(x);
		return C;
	}
	return C;
}
