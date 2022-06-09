#ifndef _ptcRUN_h_
#define _ptcRUN_h_

#include "particle.h"
#include "readINPUT.h"
#include "cell.h"
#include "readCX.h"
#include "tally.h"
extern rng RN;

class ptcRUN{
public:
	particle ptc;
	//CriData	data_rollback;
	int index, matRUN, EgroupRUN;
	bool cyl_check, pinref_check;// if ptc in cyl ->1. if ptc in pinref ->1

	bool no_sol;// det<0. no solution-> no_sol =1, else no_sol=0
	int outside_check[7];// x left right, y bottom top, z bottom top
	int next_surface;
	double sol[2];

	ptcRUN();
	~ptcRUN();
	void CAL_outside_location(readINPUT &input, cell *pin);
	double outside_location[6];// x left right, y bottom top, z bottom top
	void simulateParticle(readINPUT &input, cell *pin, readCX *CX, CriData Data, CriticalBank &CriBank, double inweight, tally &tall, bool tflag);
	void alpha_simulateParticle(readINPUT & input, cell * pin, readCX * CX, CriData Data, CriticalBank & CriBank, CriticalBank &FissionBank, double inweight, tally & tall, bool tflag);
	void eq_2nd(double a, double b, double c);
	int find_Location(particle &part, readINPUT &input, cell *pin);
	void BCtest(particle &part, readINPUT &input, cell *pin);
	double DTS(particle &part, readINPUT &input, cell *pin );
	//double DTC(particle &part, readINPUT &input, cell *pin , readCX *CX);
	int find_Group(particle &part, readINPUT &input, cell *pin , readCX *CX);
	int colType(readCX *CX);
	void ptcWeight(particle &part, readINPUT &input, cell *pin, readCX *CX);
	double fis_En(particle &part, readINPUT &input, cell *pin , readCX *CX);
	double scat_En(particle &part, readINPUT &input, cell *pin , readCX *CX);
	//void roll_back(particle &part, readINPUT &input, cell *pin,CriData &Data,readCX *CX,double inweight);
	void updateTally(readINPUT &input, cell *pin, readCX *CX, tally &tall, double distance, double inweight);
	void alpha_esti_C(particle & part, readINPUT & input, cell * pin, readCX * CX, tally & tal, double inweight);
	void esti_C(particle &part, readINPUT &input, cell *pin, readCX *CX, tally &tal,double inweight );
	void esti_T(particle &part, readINPUT &input, cell *pin, readCX *CX, tally &tal, double inweight,double distance);
	void flight(double distance, particle &part);
	void ptc2Data(CriData &Data, particle part);
	void Data2ptc(CriData &Data, particle part);
	//private:

	


	

};

#endif
