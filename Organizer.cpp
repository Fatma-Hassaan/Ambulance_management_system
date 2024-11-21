#include "Organizer.h"

Organizer::Organizer() : patientCount(0), carCount(0) {
    hospitals[0] = Hospital(1, "Hos A", 0, 0);
    hospitals[1] = Hospital(2, "Hos B", 5, 5);
    hospitals[2] = Hospital(3, "Hos C", 10, 10);
    hospitals[3] = Hospital(4, "Hos D", 15, 15);
    patients = new Patient[1000];
}
Organizer::~Organizer() {
    delete[] patients; 
}
void Organizer::calculateDistances() {
    for (int i = 0; i < 4; i++) { 
        for (int j = 0; j < patientCount; j++) { 
            distances[i][j] = sqrt(pow(hospitals[i].x - patients[j].x, 2) +
                                   pow(hospitals[i].y - patients[j].y, 2));
        }
    }
}
void Organizer::displayDistances() {
    cout << "Distance Matrix:\n";
    for (int i = 0; i < 4; i++) {
        cout << "Hospital " << hospitals[i].name << ": ";
        for (int j = 0; j < patientCount; j++) {
            cout << distances[i][j] << " ";
        }
    }
}
