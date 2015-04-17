/** Victor Wu
  CS320 Project 2
 **/

#include <string.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <math.h>
using namespace std;

bool direct_mapped_cache() {


}

int main(int argc, char *argv[]) {
	if (argc == 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h"))) cout << "Usage: wu_prj1 [INPUT] [OUTPUT]" << endl;
	else if (argc != 3) cout << "Invalid arguments. Use command --help for more information" << endl;
	else {
		//Declarations
		ifstream myFile(argv[1]);
		unsigned long long myLine = 0;
		string line;
		int hitRates[6] = {0};

		//Main Loop, reading the file
		if (myFile.is_open()) {
			while (getline(myFile,line)) {
				istringstream iss(line);
				iss >> hex >> myLine;
				//Calling of cache hit rates
				hitRates[0] += direct_mapped_cache();
			}

			//Output
			for (int i = 0; i < 6; ++i)
				cout << hitRates[i] << endl;
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
