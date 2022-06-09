#ifndef _readINPUT_h_
#define _readINPUT_h_

#include <iostream>
#include <strstream>
#include <fstream>
#include <strstream>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sstream>

using namespace std;


class cell;
class readINPUT{
public:
	
	int pre_alpha;
	int *mat_index;
	string *mater_str;

	int mat_max;
	//Boundary Condition
	int bc[6];// x left right, y bottom top, z bottom top
	
	//Region Information
	int region_fu[4];
	int region_re[4];
	
	
	//for find_Location

	double x_len_fi, y_len_fi;
	int x_num_fi, y_num_fi;
	
	//Option
	int particle_num;
	int mode; // 1: C_SOURCE, 2: S_SOURCE mode
	double x_s, xh_s, y_s, yh_s, z_s, zh_s;
	double s_en;//source energy
	int actCycle, inactCycle;
	int implicit;
	//
	cell *pin;

	readINPUT();
	~readINPUT();

	void INPUT(string name);
	void writeINPUT(ofstream &outp);
	
//private:
};

#endif
