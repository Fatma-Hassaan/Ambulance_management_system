#include "CancelQueue.h"
#include "Car.h"
#include "Hospital.h"
#include "Patient.h"
#include "TimeStep.h"

bool CancelQueue::cancelBeforeCarMoves(Patient* p, priQueue<Car*>& OC, priQueue<Car*>& BC) {
	Node<Patient*>* temp = frontPtr;
	Node<Patient*>* prev = nullptr;

	while (!temp = nullptr) {

		/// not assigned 
		bool carAssigned = false; 
		if (temp->getItem == p) {
			// find if it is in OC 
			for (int i = 0; i < OC.getSize;i++) {
				Car* C = OC.peek(i);
				if (C->getPatient == p) {
					carAssigned = true; 
					break;
				}
			}
			for (int i = 0; i < BC.getSize; i++) {
				Car* C = BC.peek(i); 
				if (C->getPatient == p) {
					carAssigned = true; 
					break;
				}


			}
			if (!carAssigned) {
				if (prev == nullptr) {
					temp = temp->getNext; 
				}
				else {
				prev->setNext(temp->getNext)
				}
				delete temp; 
				return true;
			}

		}
		prev = temp; 
		temp = temp->getNext; 
	}
	return false;
}

bool CancelQueue::cancelBeforePickup(Patient * P, priQueue<Car*>&OC, priQueue<Car*>&BC) {
	/*Car* C = nullptr;
	if (OC->setPatient(P) && !BC->setPatient(P)) {
		OC.dequeue(C);
		BC.enqueue(C, -(C->getFinishTime()));
		dequeue(P);
		return true;
	}
	return false;*/
	bool Assignedtobc = false;
	while (!Assignedtobc) {
     // loop to find it in oc 
		for (int i = 0; i < OC.getSize; i++) {
			Car* c = OC.peek(i);

			if (c->getPatient() == P) {
				// check if it not assigned to BC till now 
				for (int j = 0; j < BC.getSize; j++) {
					Car* bc = BC.peek(j);
					if (bc->getPatient()==P){
					  bool Assignedtobc = true; 
					  break;
					}
				}
				if (!Assignedtobc) {
					OC.dequeue(c);
					BC.enqueue(c);
					dequeue(P); 
					return true;
				}
			}
		}
	}

	return false; 
}

bool CancelQueue:: CancelOnCar(Patient* P, priQueue<Car*>& OC, priQueue<Car*>& BC) {
	/*Car* C = nullptr;
	if (OC->setPatient(P) || BC->setPatient(P)) {
		OC.dequeue(C);
		BC.enqueue(C, -(C->getFinishTime()));
		dequeue(P);
		return true;
	}*/

	for (int i = 0; i < BC.getSize; i++) {
		Car* bc = BC.peek(i);
		if (bc->getPatient() == P) {
			BC.dequeue(bc);
			BC.enqueue(bc, -bc->getFinishTime);
			dequeue(P);
			return true;
		}
	}
	return false;
}
