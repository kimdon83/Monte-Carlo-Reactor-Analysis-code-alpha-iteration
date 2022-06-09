#ifndef _mcman_h_
#define _mcman_h_

#include "source.h"
#include "readINPUT.h"
#include "ptcRUN.h"
#include "particle.h"

class mcman{
public:
	string inputName;
	string outputName;
	ofstream outp;
	

public:
	readINPUT inn;
	ptcRUN RUN;
	source source1;
	CriData Data;



	readCX *multiCX;
	tally tall;
	CriticalBank CriBank;
	CriticalBank FissionBank;

	double *flux;
	mcman();
	~mcman();


	int mcRUN(void);
	void eigenMode(readINPUT &input, readCX *multiCX,ptcRUN &RUN);
	void alpha_eigenMode(readINPUT & input, readCX * multiCX, ptcRUN & RUN);
private:
	chrono::system_clock::time_point start;
	chrono::system_clock::time_point end;
	chrono::duration<double> time;
};
#endif
