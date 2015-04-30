/** Victor Wu
  CS320 Project 2
 **/
#define CACHE_LINE_SIZE 32
#define BYTES 1024

#include <string.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <array>
#include <math.h>
using namespace std;

struct pCache{
	long tag;
	int sets;
	pCache() : sets(2) {
		array<int, sets> pArray;
	}
};


typedef unordered_map<unsigned long, unsigned long> Cache;
bool direct_mapped_cache(int kbSize, unsigned long long data, Cache& myCache) {
	int offset_size = log (CACHE_LINE_SIZE) / log (2.);
	int slot_size = log (BYTES * kbSize / CACHE_LINE_SIZE) / log (2.);
	unsigned long slotBits = (data >> offset_size) & ((0x1 << slot_size) - 1); 
	unsigned long tagBits = data >> (offset_size + slot_size);
	Cache::const_iterator got = myCache.find(slotBits);
	if (got != myCache.end() && got->second == tagBits) 
		return true;
	else {  myCache[slotBits] = tagBits;
	}
	return false;
}

typedef unordered_map<struct pCache, unsigned long> Cache_p;
bool setAssociative_mapped_cache(int kbSize, int sets, unsigned long long data, Cache_p& myCache) {
	int offset_size = log (DIRECT_CACHE_LINE_SIZE) / log (2.);
	int slot_size = log (BYTES * kbSize / DIRECT_CACHE_LINE_SIZE * sets) / log (2.);
	unsigned long slotBits = (data >> offset_size) & ((0x1 << slot_size) - 1);
	unsigned long tagBits = data >> (offset_size + slot_size);
	Cache::const_iterator got = myCache.find(slotBits);
	if (got != myCache.end() && got->second == tagBits)
		return true;
	else {

	}
	return false;	


return true;
}

int main(int argc, char *argv[]) {
	if (argc == 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))) cout << "Usage: wu_prj1 [INPUT] [OUTPUT]" << endl;
	else if (argc != 3) cout << "Invalid arguments. Use command --help for more information" << endl;
	else {
		//Declarations
		ifstream myFile(argv[1]);
		unsigned long long myLine = 0;
		string line;
		char loadStore = NULL;
		int hitRates[6] = {0}, totalHits = 0;
		Cache directCache_1, directCache_4,directCache_16, directCache_32;
		Cache setCacheTwo_16, setCacheFour_16, setCacheEight_16, setCacheSixteen_16;
		

		//Main Loop, reading the file
		if (myFile.is_open()) {
			while (getline(myFile,line)) {
				istringstream iss(line);
				iss >> loadStore >> hex >> myLine;	
				//Direct mapped cache
				hitRates[0] += direct_mapped_cache(1, myLine, directCache_1);
				hitRates[1] += direct_mapped_cache(4, myLine, directCache_4);
				hitRates[2] += direct_mapped_cache(16, myLine, directCache_16);
				hitRates[3] += direct_mapped_cache(32, myLine, directCache_32);

				//Set associative mapped cache
				hitRates[4] += setAssociative_mapped_cache(16, 2, myLine, setCacheTwo_16);
				hitRates[5] += setAssociative_mapped_cache(16, 4, myLine, setCacheFour_16);
				hitRates[6] += setAssociative_mapped_cache(16, 8, myLine, setCacheEight_16);
				hitRates[7] += setAssociative_mapped_cache(16, 16, myLine, setCacheSixteen_16);
				++totalHits;
			}

			//Output	
			for (int i = 0; i < 4; ++i)
				cout << hitRates[i] << ',' << totalHits << "; ";
			cout << endl;
			myFile.close();
		}
		else {
			cerr << "Unable to open input file" << endl;
			return -1;
		}


		return 0;
	}
	return -1;
}
