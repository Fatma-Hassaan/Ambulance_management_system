#include "UI.h"
#include <iostream>
#include <string>
#include "Organizer.h"
#include <fstream>
using namespace std;

void UI_::simulate(bool silent) {
    string fn = "SampleInputFile.txt";  
    fstream inputfile;
    Organizer O(inputfile, fn, silent); 
    O.incrementTimeStep_and_Execute();
}