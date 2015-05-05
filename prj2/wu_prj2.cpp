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
#include <list>
#include <math.h>
using namespace std;

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

typedef unordered_map<unsigned long, list<unsigned long> > Cache_p;
bool setAssociative_mapped_cache(int kbSize, int sets, unsigned long long data, Cache_p& myCache, bool load = true, int option = 0) {
	int offset_size = log (CACHE_LINE_SIZE) / log (2.);
	int slot_size = log (BYTES * kbSize / CACHE_LINE_SIZE * sets) / log (2.);	
	unsigned long slotBits = (data >> offset_size) & ((0x1 << slot_size) - 1);
	unsigned long tagBits = data >> (offset_size + slot_size);
	Cache_p::iterator got = myCache.find(slotBits);
	if (got != myCache.end()) {
		for (list<unsigned long>::iterator it = got->second.begin(); it != got->second.end(); ++it) {
			if (*it == tagBits) {
				got->second.remove(tagBits);
				got->second.push_back(tagBits);
				return true;
			}
		}
	}
	else if (load){
		if (myCache[slotBits].size() >= sets)
			myCache[slotBits].pop_front();
		myCache[slotBits].push_back(tagBits);
	}
	if (option) {
		if (myCache[slotBits+1].size() >= sets)
			myCache[slotBits+1].pop_front();

		myCache[slotBits+1].push_back(tagBits);
	}
	return false;	
}


bool fullAssociative_mapped_cache(int kbSize, unsigned long long data, list<unsigned long>& myCache, bool rand_opt) {		
	int offset_size = log (CACHE_LINE_SIZE) / log (2.);
	int size = BYTES * kbSize / CACHE_LINE_SIZE;
	unsigned long tagBits = data >> (offset_size);
	list<unsigned long>::iterator it;
	for (it = myCache.begin(); it != myCache.end(); ++it) {
		if (*it == tagBits) {
			myCache.remove(tagBits);
			myCache.push_back(tagBits);
			return true;
		}
	}
	if (!rand_opt) {
		if (myCache.size() >= size)
			myCache.pop_front();	
	}
	else {
		if (myCache.size() >= size) {
			it = myCache.begin();
			for (int i = 0; i < rand() % size; ++i)
				++it;
			myCache.erase(it);
		}	
	}
	myCache.push_back(tagBits);	
	return false;
}

int main(int argc, char *argv[]) {
	if (argc == 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))) cout << "Usage: wu_prj1 [INPUT] [OUTPUT]" << endl;
	else if (argc != 3) cout << "Invalid arguments. Use command --help for more information" << endl;
	else {
		//Declarations
		ifstream myFile(argv[1]);
		ofstream myFile_out(argv[2]);
		unsigned long long myLine = 0;
		string line;
		char loadStore = NULL;
		int hitRates[22] = {0}, totalHits = 0;
		Cache directCache_1, directCache_4,directCache_16, directCache_32;
		Cache_p setCacheTwo_16, setCacheFour_16, setCacheEight_16, setCacheSixteen_16;
		list<unsigned long> fullCache, fullCache_rand;	
		Cache_p setCacheTwo_16_S, setCacheFour_16_S, setCacheEight_16_S, setCacheSixteen_16_S;
		Cache_p setCacheTwo_16_P, setCacheFour_16_P, setCacheEight_16_P, setCacheSixteen_16_P;
		Cache_p setCacheTwo_16_P_m, setCacheFour_16_P_m, setCacheEight_16_P_m, setCacheSixteen_16_P_m;

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
				hitRates[4] += setAssociative_mapped_cache(16, 2, myLine, setCacheTwo_16, true);
				hitRates[5] += setAssociative_mapped_cache(16, 4, myLine, setCacheFour_16, true);
				hitRates[6] += setAssociative_mapped_cache(16, 8, myLine, setCacheEight_16, true);
				hitRates[7] += setAssociative_mapped_cache(16, 16, myLine, setCacheSixteen_16, true);

				//Fully associative mapped cache
				hitRates[8] += fullAssociative_mapped_cache(16, myLine, fullCache, false);
				hitRates[9] += fullAssociative_mapped_cache(16, myLine, fullCache_rand, true);

				//Set associative mapped cache with no allocation on a write miss
				hitRates[10] += setAssociative_mapped_cache(16, 2, myLine, setCacheTwo_16_S, loadStore == 'L' ? 1 : 0);
				hitRates[11] += setAssociative_mapped_cache(16, 4, myLine, setCacheFour_16_S, loadStore == 'L' ? 1 : 0);
				hitRates[12] += setAssociative_mapped_cache(16, 8, myLine, setCacheEight_16_S, loadStore == 'L' ? 1 : 0);
				hitRates[13] += setAssociative_mapped_cache(16, 16, myLine, setCacheSixteen_16_S, loadStore == 'L' ? 1 :0);

				//Set associative mappd cache with pre-fetching
				hitRates[14] += setAssociative_mapped_cache(16, 2, myLine, setCacheTwo_16_P, true, 1);
				hitRates[15] += setAssociative_mapped_cache(16, 2, myLine, setCacheFour_16_P, true, 1);
				hitRates[16] += setAssociative_mapped_cache(16, 2, myLine, setCacheEight_16_P, true, 1);
				hitRates[17] += setAssociative_mapped_cache(16, 2, myLine, setCacheSixteen_16_P, true, 1);

				//Set associative mapped cache with pre-fetching on miss only
				hitRates[18] += setAssociative_mapped_cache(16, 2, myLine, setCacheTwo_16_P_m, true), 2;
				hitRates[19] += setAssociative_mapped_cache(16, 2, myLine, setCacheTwo_16_P_m, true, 2);
				hitRates[20] += setAssociative_mapped_cache(16, 2, myLine, setCacheTwo_16_P_m, true, 2);
				hitRates[21] += setAssociative_mapped_cache(16, 2, myLine, setCacheTwo_16_P_m, true, 2);
				++totalHits;
			}

			myFile.close();
			//Output
			int i = 0;
			for (; i < 4; ++i)
				myFile_out << hitRates[i] << ',' << totalHits << "; ";
			myFile_out << endl;
			for (; i < 8; ++i)
				myFile_out << hitRates[i] << ',' << totalHits << "; ";
			myFile_out << endl;
			myFile_out << hitRates[i++] << endl << hitRates[i++] << endl;
			for (; i < 14; ++i)
				myFile_out << hitRates[i] << ',' << totalHits << "; ";
			myFile_out << endl;
			for (; i < 18; ++i)
				myFile_out << hitRates[i] << ',' << totalHits << "; ";
			myFile_out << endl;
			for (; i < 22; ++i)
				myFile_out << hitRates[i] << ',' << totalHits << "; ";
			myFile_out << endl;
			myFile_out.close();
		}
		else {
			cerr << "Unable to open input file" << endl;
			return -1;
		}


		return 0;
	}
	return -1;
}
