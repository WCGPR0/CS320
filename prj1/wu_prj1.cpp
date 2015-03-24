#include <string.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

#define SIZES 6

//template <typename T,unsigned S>
//inline unsigned arraysize(const T (&v)[S]) { return S; }


/// Bimodal  Predictor with a single bit of history
/** \param tableSize integer that is the size of the Table */
int bimodalOne(vector<bool> &table[SIZES], int bitSize, unsigned long long &myLine, string &myAction, int &count) {
int index = (int) myLine & 0x1 << bitSize;
//Compares Table Taken with Action Taken
if ((table[index] == 0) != (myAction == "T"))
	++count;
return 0;
} 

/// Bimodal  Predictor with a two bit of history
/** \param tableSize integer that is the size of the Table */
int bimodalTwo(vector<bool> &table[SIZES], int bitSize, unsigned long long &myLine, string &myAction, bool &myFlag, int &count) {
int index = (int) myLine & 0x1 << bitSize;
//Compares Table Taken with Action Taken
if (table[index] == 0) {
	//State TT in State Machine
	if ((myFlag == true) && (myAction == "NT"))	
		myFlag = false;
	//State T in State Machine
	else if (myFlag == false) {
		if (myAction == "NT") {
		table[index] = 1;
		++count;	
		}
		else myFlag = !(myFlag);
	}
}
else {
	//State NN in State Machine
	if ((myFlag == true) && (myAction == "T"))
		myFlag = false;
	//State N in State Machine
	else if (myFlag == false) {
		if (myAction == "T") {
		table[index] = 0;
		++count;
		}
		else myFlag = !(myFlag);
	}	
}
return 0;
} 

/*
/// GShare Predictor
int gShare(bool *table, unsigned long long &myLine, string &myAction, int &globalHistory, int *count) {
int index = (int) globalHistory ^ myLine;
if (((table[index] == 0) && (myAction == "NT")) || ((table[index] == 1) && (myAction == "T"))) {
	table[index] = !table[index];
	*count++;
}
}

/// Tournament Predictor
int tournamentPredictor(bool *table, int bitSize, unsigned long long &myLine, string &myAction, bool *myFlag, int *count, int T1, int T2) {
int index = (int) myLine & 0x1 << bitSize;
if (table[index] == 0) {
//State PG in State Machine
	if ((*myFlag == true) && ((T1 == 0) && (T2 != 0)))
			*myFlag = !(*myFlag);
//State G in State Machine
	else if (*myFlag == false) {
		if ((T1 == 0) && (T2 != 0)) {
		*count++;
		
		}
	}
}
else {
//State BB in State Machine
	if ((*myFlag == true) && ((T1 == 1) && (T2 != 1)))
			*myFlag = !(*myFlag);
//State BG in State Machine
	else if (*myFlag == false) {
		if ((T1 == 1) && (T2 != 1))
		*count++;
	}	
}
return 0;
}
*/
int main(int argc, char *argv[]) {
if (argc == 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))) cout << "Usage: wu_prj1 [INPUT] [OUTPUT]" << endl;
else if (argc != 3) cout << "Invalid arguments. Use command --help for more information" << endl;
else {

//Declarations
ifstream myFile(argv[1]);
unsigned long long myLine = 0;
string myAction, line;
int branches = 0, always_taken_count = 0, not_taken_count = 0, bimodalOne_count[SIZES] = {0}, bimodalTwo_count[SIZES] = {0}, global_count = 0, tournament_count = 0;
//bool *myBooleanTable[SIZES], *myBooleanTable2[SIZES], *myBooleanTable3[SIZES], gShareTable[2048] = {0}, tournamentTable[2048] = {0};
vector<bool> bimodalOneTable[SIZES], bimodalTwoTable[SIZES];

register int globalHistory = 0;
bool gFlag = 0, tournamentFlag = 0;
// \brief strong flags
bool bimodalTwo_flags[SIZES];
enum States { strongG, weakG, weakB, strongB };
States tournamentStates[6];

//Initialization
int array[] = {16, 32, 128, 256, 512, 1024, 2048};
for (int i = 0; i < SIZES; i++) {
bimodalOneTable[i].assign(array[i], 1);
bimodalTwoTable[i].assign(array[i], 1);
}

fill_n(gShareTable, 2048, 1);

//Main Loop, reading the file
if (myFile.is_open()) {
	while (getline(myFile,line)) {
		istringstream iss(line);
		iss >> hex >> myLine >> myAction; //<< myAction << myLine << hex << line;	
		//Always Taken
		if (myAction == "T") ++always_taken_count;
		else if (myAction == "NT") ++not_taken_count;
		else cerr << "Error reading Taken/Not Taken" << endl;
		int T1, T2;	
		for (int i = 0; i < SIZES; i ++) {
			bimodalOne(bimodalOneTable, array[i],  myLine, myAction, bimodalOne_count[i]);
			bimodalTwo(bimodalTwoTable, array[i],  myLine, myAction, bimodalTwo_flags[i], bimodalTwo_count[i]);
		}	
		//T2 = gShare(gShareTable, myLine, myAction, globalHistory, &global_count);
		//tournamentPredictor(tournamentTable, myLine, myAction, &tournament_count, T1, T2);
		++branches;
	}

//Output
cout << always_taken_count << ',' << branches << ';' << endl;
cout << not_taken_count << ',' << branches << ';' << endl;
for (int i = 0; i < SIZES; i++)
	cout << bimodalOne_count[i] << ',' << branches << ';';
	cout << endl;
for (int i = 0; i < SIZES; i++)
	cout << bimodalTwo_count[i] << ',' << branches << ';';
	cout << endl;
	myFile.close();
	
}
else {
cerr << "Unable to open input file" << endl;
return -1;
}


//CleanUp
for (int i = 0; i < SIZES; i++)
	delete myBooleanTable[i];
//delete myBooleanTable;

return 0;
}
return -1;
}
