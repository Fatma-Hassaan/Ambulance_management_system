# pragma once 
#include "Hospital.h"

class RedistMat {
private:
	int** distanceMat; 
	int numOfHospitals;
	Hospital** hos; 
public:
	RedistMat(int numOfHospitals, int** distance, Hospital** hos);
	~RedistMat(); 
	bool checknearHos(Patient* P); 
	void redistpatient(Patient* P);
	void updateMat(Patient* P, int x);
};
