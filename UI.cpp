#include "UI.h"
#include <iostream>
#include <string>
#include "Organizer.h"
#include <fstream>
using namespace std;

void UI_::simulate() {
	string fn;
	cout << "Enter the file name: ";
	cin >> fn;
	fstream inputfile;
	Organizer O(inputfile, fn);
	O.incrementTimeStep_and_Execute();
}