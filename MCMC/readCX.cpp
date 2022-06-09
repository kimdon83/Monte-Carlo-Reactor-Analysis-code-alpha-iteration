#include <iomanip>
#include <iostream>
#include <strstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sstream>

#include "readCX.h"
using namespace std;

readCX::readCX(){
	//int step_len, j_final, stopper, step_start, step_final, err_counter;
}
readCX::~readCX(){
	//int step_len, j_final, stopper, step_start, step_final, err_counter;
}
void readCX::read(int name){
	//	copy CX file to buff	
	string s, s1;

	stringstream ss;
	ss << name << ".txt";
	s = ss.str();
	cout << "reading " << s << endl;///
	ifstream infp;
	infp.open(s);

	int loc = infp.tellg();
	infp.seekg(0, ios::end);
	int filesize = infp.tellg();
	infp.seekg(loc);

	char *buff = new char[filesize];
	infp.read(buff, filesize);
	infp.close();

	istrstream inFp(buff);
	char *tmp;
	char line[265];
	// read energy boundary
	
	// read CX and Nu
	tmp = strstr(buff, "XS");
	inFp.seekg((int)(tmp - buff));
	inFp >> line;
	
	for (int i = 0; i < nGrp; i++) {
		inFp >> line;
		trans[i] = atof(line); // transport CX
	}

	for (int i = 0; i < nGrp; i++) {
		inFp >> line;
		fission[i] = atof(line); // fisstion CX
	}
	for (int i = 0; i < nGrp; i++){
		inFp >> line;
		Nu[i] = atof(line); // Nu CX
	}
	for (int i = 0; i < nGrp; i++) {
		inFp >> line;
		inv_velocity[i] = atof(line); // Nu CX
	}
	for (int i = 0; i < nGrp; i++) {
		inFp >> line;
		chi[i] = atof(line); // Nu CX
	}


	// read scattering CX (a to b)
	tmp = strstr(buff, "Scattering");
	inFp.seekg((int)(tmp - buff));
	inFp >> line;
	
	for (int i = 0; i < nGrp; i++){
		for (int j = 0; j < nGrp; j++) {
			inFp >> line;
			scatter[i][j] = atof(line);// scatter i to j
		}
		total_scatter[i] = 0;
	}
	for (int i = 0; i < nGrp; i++) {
		for (int j = 0; j < nGrp; j++) {
			total_scatter[i] += scatter[i][j];
		}
	}


	delete[] buff;
	buff = NULL;
}
