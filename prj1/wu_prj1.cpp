#include <string.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <math.h>
using namespace std;

#define SIZES 6

//template <typename T,unsigned S>
//inline unsigned arraysize(const T (&v)[S]) { return S; }

enum preferStates { GG, G, B, BB};
enum biStates {NT, T};

/// Helper function for incrementing bitsets
template <size_t N>
std::bitset<N> increment (std::bitset<N> bit) {
	std::bitset<N> max;
	max.set();
	if (bit == max) return bit; //If bit is at max value, does nothing
    for ( size_t i = 0; i < N; ++i ) {
        if ( bit[i] == 0 ) {  //No carry
            bit[i] = 1;
            break;
            }
        bit[i] = 0;  //Carry 
        }
    return bit;
}

/// Helper function for decrementing bitsets
template <size_t N>
std::bitset<N> decrement (std::bitset<N> bit) {
	std::bitset<N> min; 
	if (bit == min) return bit; //If bit is 0, does nothing
    for ( size_t i = 0; i < N; ++i ) {
	bit[i] = bit[i] ^ 1;
        if ( bit[i] == 0 ) {  //No carry
            bit[i] = 0;
            break;
            } 
        }
    return bit;
}


/// Bimodal  Predictor with a single bit of history
/** \param tableSize integer that is the size of the Table */
int bimodalOne(vector<bitset<1> > &table, int bitShift, unsigned long long &myLine, string &myAction, int &count) {
int index = (int) (0x1 << bitShift) - 1 & myLine;
//Compares Table Taken with Action Taken
if (((table[index] == T) && (myAction == "NT")) || ((table[index] == NT) && (myAction == "T"))){
	++count;
	table[index] = ~table[index]; 
}
return (int) table[index].to_ulong();
} 

/// Bimodal  Predictor with a two bit of history
/** \param tableSize integer that is the size of the Table */
int bimodalTwo(vector<bitset<2> > &table, int bitShift, unsigned long long &myLine, string &myAction, int &count) {
int index = (int) (0x1 << bitShift) - 1 & myLine;
bool action = (myAction == "T") ? true : false;
unsigned long value = table[index].to_ulong();
if (!action) {
if (value > 0) {
	if (value == 1) ++count; //Transition from State T to N 
	table[index] = increment(table[index]);	
	}
}
else if (value < 4) {
	if (value == 2) ++count; //Transition from State N to T
	table[index] = decrement(table[index]);
	}

return (int) table[index].to_ulong();
} 


/// GShare Predictor
int gShare(vector<bitset<2> > &table, int bitSize, unsigned long long &myLine, string &myAction, unsigned long &globalHistory, int &historySize,  int &count) {
unsigned int index = (unsigned int) globalHistory << (bitSize - historySize++) ^ (myLine & ((1 << bitSize) - 1));
unsigned long value = table[index].to_ulong();
bool action = (myAction == "T") ? 1 : 0;
if (!action) {
if (value > 0) {
	if (value == 1) ++count; //Transition from State T to N 
	table[index] = increment(table[index]);	
	}
}
else if (value < 3) {
	if (value == 2) ++count; //Transition from State N to T
	table[index] = decrement(table[index]);
	}

globalHistory <<= 1;
globalHistory |= table[index].to_ulong();
globalHistory &= (1 << historySize)-1; 

return (int) globalHistory;
}

/*
/// Tournament Predictor
int tournamentPredictor(vector<bitsize<2> > &table, int bitSize, unsigned long long &myLine, string &myAction, int &count, int T1, int T2) {
int index = (int) (0x1 << bitSize) & myLine;

bool action = (myAction == "T") ? 1 : 0;

if (T1 != T2) {
	if (T1 == (int) action) 
	if (table[index] == 1 && T2) { //Transition from prefer G to weak G
		
	}

}

return 0;
} */

int main(int argc, char *argv[]) {
if (argc == 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))) cout << "Usage: wu_prj1 [INPUT] [OUTPUT]" << endl;
else if (argc != 3) cout << "Invalid arguments. Use command --help for more information" << endl;
else {

//Declarations
ifstream myFile(argv[1]);
unsigned long long myLine = 0;
string myAction, line;
int branches = 0, always_taken_count = 0, not_taken_count = 0, bimodalOne_count[SIZES] = {0}, bimodalTwo_count[SIZES] = {0}, global_count = 0, tournament_count = 0, historyCount = 3;

register unsigned long globalHistory = 0;
bool gFlag = 0, tournamentFlag = 0;
// \brief strong flags
bool bimodalTwo_flags[SIZES];
bitset<2> myTwoState(GG);
bitset<1> myOneState(T);

vector<bitset<1> > bimodalOneTable[SIZES];
vector<bitset<2> > bimodalTwoTable[SIZES], gShareTable;

//Initialization
int array[] = {16, 32, 128, 256, 512, 1024, 2048};
for (int i = 0; i < SIZES; i++) {
bimodalOneTable[i].assign(array[i], myOneState);
bimodalTwoTable[i].assign(array[i], myTwoState);
}

gShareTable.assign(array[SIZES], myTwoState);

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
		int arraySizes[] = {4, 5, 7, 8, 9, 10, 11};
			bimodalOne(bimodalOneTable[i], arraySizes[i],  myLine, myAction, bimodalOne_count[i]);
			bimodalTwo(bimodalTwoTable[i], arraySizes[i],  myLine, myAction, bimodalTwo_count[i]);
		}	
		//T2 = gShare(gShareTable,array[SIZES], myLine, myAction, globalHistory, historyCount, global_count);
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

//cout << global_count << ',' << branches << ';' << endl; 
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
