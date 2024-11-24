#include "hospital.h"
#include "patient.h"
#include "Car.h"
#include <iostream>
#include <cmath> 
using namespace std;
class Organizer {
private:
    Hospital hospitals[4];       
    Patient* patients;           
    double distances[4][1000]; 
    int patientCount;
    int carCount;
public:
    Organizer();
    ~Organizer();
    void addpatient(int pid, string type, double x, double y);
    void calculateDistances();
    void displayDistances();
};
