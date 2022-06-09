#include <iomanip>
#include <iostream>
#include <strstream>
#include <fstream>
#include <strstream>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sstream>

#include "readINPUT.h"
#include "cell.h"
#include "ptcRUN.h"
#include "mcman.h"

#include "define.h"

using namespace std;

readINPUT::readINPUT(){

}
readINPUT::~readINPUT(){
	//delete[] mat_index;		mat_index=NULL;
	//delete[] mater_str;		mater_str=NULL;
	//delete[] pin;			pin=NULL;
}
void readINPUT::writeINPUT(ofstream &outp){

	outp <<"******************************INPUT******************************" << endl;

	string s, temp;

	stringstream ss;
	//ss << "input_uo2.txt";
	ss << "input.txt";
	//ss << "input_MOX87rectang1.txt";
	s = ss.str();
	cout << "reading " << s << endl;
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
	char line[256];

	for (int i = 0; i < 17; i++){
		inFp.getline(line, 256);
		outp << line << endl;

	}
	
}

void readINPUT::INPUT(string name){
	int i = 0, j = 0;

	//	copy CX file to buff	
	string s, temp;

	stringstream ss;
	//ss << "input_uo2.txt";
	//ss << "input.txt";
	ss << name;
	//ss << "test.txt";
	//ss << "input_MOX87rectang1.txt";
	s = ss.str();
	cout << "reading " << s << endl;
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
	char line[256];

	// read Material Index of cyl region
	int index_max = 10000;
	int *mtc_cyl = new int[index_max];

	tmp = strstr(buff, "Material_Index1");
	if (tmp != NULL){
		inFp.seekg((int)(tmp - buff));
		inFp >> line;
		inFp >> line;
		inFp >> line;
		i = 0;
		while (line[1] != ';'){
			mtc_cyl[i] = atoi(line);

			i++;
			inFp >> line;
		}
	}
	// read Material Index of out of cyl Fuel region
	int *mtc_pinout = new int[index_max];

	tmp = strstr(buff, "Material_Index2");
	if (tmp != NULL){
		inFp.seekg((int)(tmp - buff));
		inFp >> line;
		inFp >> line;
		inFp >> line;
		i = 0;
		while (line[1] != ';'){
			mtc_pinout[i] = atoi(line);

			i++;
			inFp >> line;
		}
	}
	// read Option
	tmp = strstr(buff, "Option");
	inFp.seekg((int)(tmp - buff));
	inFp >> line;
	inFp >> line;
	inFp >> line;

	if (!strcmp(line, "C_SOURCE")){
		mode = C_SOURCE;
	}
	else if (!strcmp(line, "S_SOURCE")){
		mode = S_SOURCE;
	}
	else {
		cout << "check the input file";
	}
	inFp >> line;			// read n
	inFp >> particle_num;
	inFp >> line;			// read source distribution type
	if (!strcmp(line, "cube")){

		inFp >> x_s;
		inFp >> xh_s;
		inFp >> y_s;
		inFp >> yh_s;
		inFp >> z_s;
		inFp >> zh_s;

	}
	else if (!strcmp(line, "PNT")){ 
		inFp >> x_s;
		inFp >> y_s;
		inFp >> z_s;
	}
	inFp >> line;		// read ENERGY
	inFp >> s_en;		// [Mev]

	inFp >> line;		// readf inactive/active
	inFp >> inactCycle;
	inFp >> actCycle;
	inFp >> line;
	if (!strcmp(line, "implicitCap")){
		implicit = 1;
	}
	else implicit = 0;
	inFp >> line;
	
	// read Material name and cmat_cyl
	tmp = strstr(buff, "Material");
	inFp.seekg((int)(tmp - buff));
	inFp >> line;
	inFp >> line;
	inFp >> line;

	i = 0;
	while (line[1] != ';'){
		inFp >> line;
		inFp >> line;
		i++;//물질 종류 수를 세기 위한 while문
	}
	mat_max = i;
	tmp = strstr(buff, "Material");
	inFp.seekg((int)(tmp - buff));
	inFp >> line;
	inFp >> line;
	inFp >> line;

	mat_index = new int[mat_max];
	mater_str = new string[mat_max];

	i = 0;
	while (line[1] != ';'){
		mater_str[i] = line;
		inFp >> line;
		mat_index[i] = atoi(line);
		inFp >> line;
		i++;
	}
	// read Cells to make every cells

	double x_len, y_len;
	int cell_number, x_num, y_num, temp_int;

	tmp = strstr(buff, "Cells");

	inFp.seekg((int)(tmp - buff));
	inFp >> line;
	inFp >> line;
	inFp >> line;

	if (!strcmp(line, "PIN")){
		inFp >> x_num;
		inFp >> y_num;
		cell_number = x_num*y_num;
		pin = new cell[cell_number];//cell *pin = new cell[cell_number];
		inFp >> line;
		x_len = atof(line);
		inFp >> line;
		y_len = atof(line);
		inFp >> line;
		for (j = 0; j < y_num; j++){
			for (i = 0; i < x_num; i++){
				pin[i + j*x_num].xl = atof(line) + x_len*i;
			}
		}
		inFp >> line;
		for (j = 0; j < y_num; j++){
			for (i = 0; i < x_num; i++){
				pin[i + j*x_num].xh = atof(line) + x_len*i;
			}
		}
		inFp >> line;
		for (j = 0; j < y_num; j++){
			for (i = 0; i < x_num; i++){
				pin[i + j*x_num].yl = atof(line) + y_len*j;
			}
		}
		inFp >> line;
		for (j = 0; j < y_num; j++){
			for (i = 0; i < x_num; i++){
				pin[i + j*x_num].yh = atof(line) + y_len*j;
			}
		}
		inFp >> line;
		for (j = 0; j < y_num; j++){
			for (i = 0; i < x_num; i++){
				pin[i + j*x_num].rx0 = atof(line);
			}
		}
		inFp >> line;
		for (j = 0; j < y_num; j++){
			for (i = 0; i < x_num; i++){
				pin[i + j*x_num].ry0 = atof(line);
			}
		}
		inFp >> line;
		for (j = 0; j < y_num; j++){
			for (i = 0; i < x_num; i++){
				pin[i + j*x_num].radius = atof(line);
			}
		}
		inFp >> line;
		for (j = 0; j < y_num; j++){
			for (i = 0; i < x_num; i++){
				pin[i + j*x_num].zl = atof(line);
			}
		}
		inFp >> line;
		for (j = 0; j < y_num; j++){
			for (i = 0; i < x_num; i++){
				pin[i + j*x_num].zh = atof(line);
			}
		}
		inFp >> line;
		for (j = 0; j < y_num; j++){
			for (i = 0; i < x_num; i++){
				pin[i + j*x_num].z_ref = atof(line);
			}
		}

		for (j = 0; j < cell_number; j++){
			pin[j].volume = x_len*y_len*(pin[j].z_ref - pin[j].zl);
		}
		inFp >> line;

		if (!strcmp(line, "Index_cyl")){
			for (j = 0; j < y_num; j++){
				for (i = 0; i < x_num; i++){
					pin[i + j*x_num].cmat_cyl = mtc_cyl[i + j*y_num];
				}
			}
		}
		else{
			for (i = 0; i < mat_max; i++){//string mater_str[10];
				if (mater_str[i] == line){
					temp_int = mat_index[i];
					break;
				}
			}
			for (j = 0; j < y_num; j++){
				for (i = 0; i < x_num; i++){
					pin[i + j*x_num].cmat_cyl = temp_int;
				}
			}
		}
		inFp >> line;

		if (!strcmp(line, "Index_moder")){
			for (j = 0; j < y_num; j++){
				for (i = 0; i < x_num; i++){
					pin[i + j*x_num].cmat_pinout = mtc_pinout[i + j*y_num];
				}
			}
		}
		else{
			for (i = 0; i < mat_max; i++){//string mater_str[10];
				if (mater_str[i] == line){
					temp_int = mat_index[i];
					break;
				}
			}
			for (j = 0; j < y_num; j++){
				for (i = 0; i < x_num; i++){
					pin[i + j*x_num].cmat_pinout = temp_int;
				}
			}
		}

		x_num_fi = x_num;
		y_num_fi = y_num;
		x_len_fi = x_len;
		y_len_fi = y_len;

		//read boundary condition
		tmp = strstr(buff, "BC");
		inFp.seekg((int)(tmp - buff));

		inFp >> line;		// read BC

		for ( i = 0; i < 6; i++){
			inFp >> line;
			if (!strcmp(line, "refl")){
				bc[i] = reflection;
			}
			else if (!strcmp(line, "zero")){
				bc[i] = zero_flux;
			}
		}

		//read fuel/reflector condition

		inFp >> line;			// read Fuel or Reflector

		while (1){
			if (!strcmp(line, "Fuel")){
				for (i = 0; i < 4; i++){
					inFp >> region_fu[i];
				}
				for (j = region_fu[0] - 1; j < region_fu[1]; j++){
					for (i = region_fu[2] - 1; i < region_fu[3]; i++){
						pin[i + j*x_num].cell_type = Fuel;
					}
				}


				inFp >> line;
			}
			else if (!strcmp(line, "Reflector")){
				for (i = 0; i < 4; i++){
					inFp >> region_re[i];
				}

				for (j = region_re[2] - 1; j < region_re[3]; j++){
					for (i = region_re[0] - 1; i < region_re[1]; i++){
						pin[i + j*x_num].cell_type = Reflector;
					}
				}

				inFp >> line;
			}
			else{
				break;
			}
		}
	}

	/*if (x_num*y_num==1){
		pin[0].Cell_index[0] = OUT;
		pin[0].Cell_index[1] = OUT;
		pin[0].Cell_index[2] = OUT;
		pin[0].Cell_index[3] = OUT;
		for (k = 0; k < 6; k++){
			pin[0].BC[k] = bc[k];
		}
	}
	else{
		for (j = 0; j < y_num; j++){
			for (i = 0; i < x_num ; i++){
				pin[i + j*x_num].BC[5] = bc[5];
				pin[i + j*x_num].BC[4] = bc[4];
			}
		}

		for (j = 1; j < y_num - 1; j++){
			for (i = 1; i < x_num - 1; i++){
				pin[i + j*x_num].Cell_index[0] = i + j*y_num - 1;
				pin[i + j*x_num].Cell_index[1] = i + j*y_num + 1;
				pin[i + j*x_num].Cell_index[2] = i + (j - 1)*y_num;
				pin[i + j*x_num].Cell_index[3] = i + (j + 1)*y_num;
				pin[i + j*x_num].BC[0] = IN;
				pin[i + j*x_num].BC[1] = IN; 
				pin[i + j*x_num].BC[2] = IN; 
				pin[i + j*x_num].BC[3] = IN; 
			}
		}

		i = 0;
		j = 0;
		pin[i + j*x_num].Cell_index[0] = OUT;
		pin[i + j*x_num].Cell_index[1] = i + j*y_num + 1;
		pin[i + j*x_num].Cell_index[2] = OUT;
		pin[i + j*x_num].Cell_index[3] = i + (j + 1)*y_num;
		pin[i + j*x_num].BC[0] = bc[0];
		pin[i + j*x_num].BC[1] = IN;
		pin[i + j*x_num].BC[2] = bc[2];
		pin[i + j*x_num].BC[3] = IN;

		i = x_num - 1;
		j = 0;
		pin[i + j*x_num].Cell_index[0] = i + j*y_num - 1;
		pin[i + j*x_num].Cell_index[1] = OUT;
		pin[i + j*x_num].Cell_index[2] = OUT;
		pin[i + j*x_num].Cell_index[3] = i + (j + 1)*y_num;
		pin[i + j*x_num].BC[0] = IN;
		pin[i + j*x_num].BC[1] = bc[1];
		pin[i + j*x_num].BC[2] = bc[2];
		pin[i + j*x_num].BC[3] = IN;

		i = 0;
		j = y_num - 1;
		pin[i + j*x_num].Cell_index[0] = OUT;
		pin[i + j*x_num].Cell_index[1] = i + j*y_num + 1;
		pin[i + j*x_num].Cell_index[2] = i + (j - 1)*y_num;
		pin[i + j*x_num].Cell_index[3] = OUT;
		pin[i + j*x_num].BC[0] = bc[0];
		pin[i + j*x_num].BC[1] = IN;
		pin[i + j*x_num].BC[2] = IN;
		pin[i + j*x_num].BC[3] = bc[3];

		i = x_num - 1;
		j = y_num - 1;
		pin[i + j*x_num].Cell_index[0] = i + j*y_num - 1;
		pin[i + j*x_num].Cell_index[1] = OUT;
		pin[i + j*x_num].Cell_index[2] = i + (j - 1)*y_num;
		pin[i + j*x_num].Cell_index[3] = OUT;
		pin[i + j*x_num].BC[0] = IN;
		pin[i + j*x_num].BC[1] = bc[1];
		pin[i + j*x_num].BC[2] = IN;
		pin[i + j*x_num].BC[3] = bc[3];

		i = 0;
		for (j = 1; j < y_num - 1; j++){
			pin[i + j*x_num].Cell_index[0] = OUT;
			pin[i + j*x_num].Cell_index[1] = i + j*y_num + 1;
			pin[i + j*x_num].Cell_index[2] = i + (j - 1)*y_num;
			pin[i + j*x_num].Cell_index[3] = i + (j + 1)*y_num;
			pin[i + j*x_num].BC[0] = bc[0];
			pin[i + j*x_num].BC[1] = IN;
			pin[i + j*x_num].BC[2] = IN;
			pin[i + j*x_num].BC[3] = IN;
		}

		i = x_num - 1;
		for (j = 1; j < y_num - 1; j++){
			pin[i + j*x_num].Cell_index[0] = i + j*y_num - 1;
			pin[i + j*x_num].Cell_index[1] = OUT;
			pin[i + j*x_num].Cell_index[2] = i + (j - 1)*y_num;
			pin[i + j*x_num].Cell_index[3] = i + (j + 1)*y_num;
			pin[i + j*x_num].BC[0] = IN;
			pin[i + j*x_num].BC[1] = bc[1];
			pin[i + j*x_num].BC[2] = IN;
			pin[i + j*x_num].BC[3] = IN;
		}

		j = 0;
		for (i = 1; i < x_num - 1; i++){
			pin[i + j*x_num].Cell_index[0] = i + j*y_num - 1;
			pin[i + j*x_num].Cell_index[1] = i + j*y_num + 1;
			pin[i + j*x_num].Cell_index[2] = OUT;
			pin[i + j*x_num].Cell_index[3] = i + (j + 1)*y_num;
			pin[i + j*x_num].BC[0] = IN;
			pin[i + j*x_num].BC[1] = IN;
			pin[i + j*x_num].BC[2] = bc[2];
			pin[i + j*x_num].BC[3] = IN;
		}

		j = y_num - 1;
			for (i = 1; i < x_num - 1; i++){
				pin[i + j*x_num].Cell_index[0] = i + j*y_num - 1;
				pin[i + j*x_num].Cell_index[1] = i + j*y_num + 1;
				pin[i + j*x_num].Cell_index[2] = i + (j - 1)*y_num;
				pin[i + j*x_num].Cell_index[3] = OUT;
				pin[i + j*x_num].BC[0] = IN;
				pin[i + j*x_num].BC[1] = IN;
				pin[i + j*x_num].BC[2] = IN;
				pin[i + j*x_num].BC[3] = bc[3];
			}
	}*/


	delete[] buff;
	buff = NULL;
	delete[] mtc_cyl;
	delete[] mtc_pinout;
}