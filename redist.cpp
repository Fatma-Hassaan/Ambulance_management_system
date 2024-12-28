#include "redist.h"
#include "organizer.h"
#include"Patient.h"


// redistrbution classmust contain :
// 1. matrix of hospital created based on distance 
// 2. constroctor for intializing the matrix 
// 3. destructor 
//4. function to check if there is a place in nearesr hospital for the emer patient 
//5. function to redistribute the patient a near hospital if the neearesr one is full 
// 6. function to update matrix for each patient place from hospitals 
// thus the algorithm will be:
//  class 
RedistMat::RedistMat(int numOfHospitals, int** distances, Hospital** hos) {
	numOfHospitals = numOfHospitals; 
	hos = hos; 
	distanceMat = new int* [numOfHospitals]; 
	for (int i = 0;i < numOfHospitals; i++) {
		distanceMat[i] = new int[numOfHospitals]; 
		 for (int j = 0; j < numOfHospitals; j++) {
			 distanceMat[i][j] = distances[i][j];
		 } 
	}
}

RedistMat::~RedistMat() {
	for (int i = 0; i < numOfHospitals; i++) {
		delete[] distanceMat[i];
	}
	delete[]distanceMat; 
}

bool RedistMat::checknearHos(Patient* P) {
    int nearhosI = -1; 
    int mindist = 0; 
	while (int nearhosI = -1) {
		for (int i = 0; i < numOfHospitals; i++) {
			if (distanceMat[P->getHID()][i] < mindist && hos[i]->getNumber_FreeNC>0 || hos[i]->getNumber_FreeSC > 0) {
				mindist = distanceMat[P->getHID()][i];
				int nearhosI = i;
				hos[i]->RecievePatient(P);
				updateMat(P, nearhosI)
				break;

			}
			else {
				return nearhosI == -1;
			}
		}
	}
    // return nearhosI != -1;	
}
/*void RedistMat::redistpatient(Patient* P) {
	int nearhosI = -1; 
	int mindist = 0; 
	int CHID = P->getHID(); 
	for (int i = 0; i < numOfHospitals; i++) {
		if (i != CHID && distanceMat[CHID][i] < mindist && hos[i]->getNumber_FreeNC>0 || hos[i]->getNumber_FreeSC > 0) {
			mindist = distanceMat[P->getHID()][i];
			int nearhosI = i;
		}
	}
}*/


void RedistMat::updateMat(Patient* P, int x) {
	hos[x]->getNumber_CurrentEP++;
	if (hos[x]->getNumber_FreeNC > 0) {
		hos[x]->getNumber_FreeNC--;
	}
	else if (hos[x]->getNumber_FreeSC > 0) {
		hos[x]->getNumber_FreeSC--;
	}
}
