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
int bimodalOne(vector<bool> &table, int bitSize, unsigned long long &myLine, string &myAction, int &count) {
int index = (int) (0x1 << bitSize) & myLine;
//Compares Table Taken with Action Taken
if ((table[index] == 0) != (myAction == "T"))
	++count;
return 0;
} 

/// Bimodal  Predictor with a two bit of history
/** \param tableSize integer that is the size of the Table */
int bimodalTwo(vector<bool> &table, int bitSize, unsigned long long &myLine, string &myAction, bool &myFlag, int &count) {
int index = (int) (0x1 << bitSize) & myLine;
//Compares Table Taken with Action Taken
if (table[index] == 1) {	
	if (myAction == "NT") { //Transition from State TT to T in State Machine
      if (myFlag)
		   myFlag = false;
      else { //Transition from State T to N in State Machine
         table[index] = 0;
         ++count;
      }	
	}
   else if (!myFlag) myFlag = !myFlag; //Transition from State T to TT
}
else {
	if (myAction == "T") { //Transition from State NN to N in State Machine
      if (myFlag)
		   myFlag = false;
      else { //Transition from State N to T in State Machine
         table[index] = 1;
         ++count;
      }
   }	
	else if (!myFlag) myFlag = !myFlag; //Transition from State N to NN	
}
return 0;
} 


/// GShare Predictor
int gShare(vector<bool> &table, int bitSize, unsigned long long &myLine, string &myAction, unsigned int &globalHistory, int &historySize, bool &myFlag, int &count) {
unsigned int index = (unsigned int) globalHistory << (bitSize - historySize++) ^ (myLine & ((1 << bitSize) - 1));

bool action = (myAction == "T") ? 1 : 0;
if (action != table[index]) {
if (!myFlag) { table[index] = !table[index]; ++count; } //Transition from State T to N or N to T
else { myFlag = !myFlag; } //Transition from State TT to T or NN to N
}
else if (!myFlag) myFlag = !myFlag; //Transition from State T to TT or N to NN
globalHistory <<= 1;
globalHistory |= action;
globalHistory &= (1 << historySize)-1; 

return table[index] | (myFlag << 1);
}

/*
/// Tournament Predictor
int tournamentPredictor(vector<bool> &table, int bitSize, unsigned long long &myLine, string &myAction, bool &myFlag, int &count, int T1, int T2) {
int index = (int) (0x1 << bitSize) & myLine;

if (T2 != T1) {
	if (!myFlag) { 
		if (T2) table[index] = 1;
		else table[index] = 0;
		++count; } 
	else { myFlag = !myFlag; } //Transition from State TT to T or NN to N
	}
else if (!myFlag) myFlag = !myFlag; //Transition from State T to TT or N to NN

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
int branches = 0, always_taken_count = 0, not_taken_count = 0, bimodalOne_count[SIZES] = {0}, bimodalTwo_count[SIZES] = {0}, global_count = 0, tournament_count = 0, historyCount = 3;
vector<bool> bimodalOneTable[SIZES], bimodalTwoTable[SIZES], gShareTable;

register unsigned int globalHistory = 0;
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

gShareTable.assign(array[SIZES], 1);

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
	/*	for (int i = 0; i < SIZES; i ++) {
			bimodalOne(bimodalOneTable[i], array[i],  myLine, myAction, bimodalOne_count[i]);
			bimodalTwo(bimodalTwoTable[i], array[i],  myLine, myAction, bimodalTwo_flags[i], bimodalTwo_count[i]);
		}	*/
		T2 = gShare(gShareTable,array[SIZES], myLine, myAction, globalHistory, historyCount, gFlag, global_count);
		//tournamentPredictor(tournamentTable, myLine, myAction, &tournament_count, T1, T2);
		++branches;
	}

//Output
/*cout << always_taken_count << ',' << branches << ';' << endl;
cout << not_taken_count << ',' << branches << ';' << endl;
for (int i = 0; i < SIZES; i++)
	cout << bimodalOne_count[i] << ',' << branches << ';';
	cout << endl;
for (int i = 0; i < SIZES; i++)
	cout << bimodalTwo_count[i] << ',' << branches << ';';
	cout << endl;
	myFile.close();
*/
cout << global_count << ',' << branches << ';' << endl; 
}
else {
cerr << "Unable to open input file" << endl;
return -1;
}


//CleanUp
//for (int i = 0; i < SIZES; i++)
//	delete myBooleanTable[i];
//delete myBooleanTable;

return 0;
}
return -1;
}
