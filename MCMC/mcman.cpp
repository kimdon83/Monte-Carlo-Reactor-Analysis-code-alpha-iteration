#include <iomanip>
#include <iostream>
#include <strstream>
#include <fstream>
#include <strstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sstream>
#include <cmath>

#include "rng.h"
#include "readCX.h"
#include "readINPUT.h"
#include "cell.h"
#include "particle.h"
#include "ptcRUN.h"
#include "source.h"
#include "mcman.h"
#include "tally.h"

using namespace std;

ofstream debug_out;



mcman::mcman() {

}
mcman::~mcman() {
	delete[] multiCX;
	delete[] flux;
}
int mcman::mcRUN(void) {
	start = chrono::system_clock::now();

	//inputName = "test.txt";
	inputName = "input.txt";
	outputName = "OUTPUT";

	inn.INPUT(inputName);

	//readCX *multiCX = new readCX[inn.mat_max];
	multiCX = new readCX[inn.mat_max];
	for (int i = 0; i < inn.mat_max; i++) {
		multiCX[i].read(inn.mat_index[i]);
	}

	RUN.CAL_outside_location(inn, inn.pin);


	for (int i = 0; i < inn.particle_num; i++) {
		source1.mk_source(inn, Data);
		CriBank.push(Data);
	}
	//debug_out.open("debuggg.txt");

	//eigenMode(inn, multiCX, RUN);
	alpha_eigenMode(inn, multiCX, RUN);

//	debug_out << 1;
	//debug_out.close();

	end = chrono::system_clock::now();
	time = end - start;
	cout << "*----------------------------------------------------------------------------*" << endl;
	cout << "      Time = " << time.count() << endl;
	cout << "******************************************************************************" << endl;

	outp << "*----------------------------------------------------------------------------*" << endl;
	outp << "      Time = " << time.count() << endl;
	outp << "******************************************************************************" << endl;

	outp.close();

	delete[] inn.mat_index;		inn.mat_index = NULL;
	delete[] inn.mater_str;		inn.mater_str = NULL;
	delete[] inn.pin;			inn.pin = NULL;


	return 1;
}

void mcman::eigenMode(readINPUT &input, readCX *multiCX, ptcRUN &RUN) {
	int i, j;
	CriData Data;

	outp.open(outputName);

	input.writeINPUT(outp);
	outp << "******************************EigenMode******************************" << endl;
	cout << "******************************EigenMode******************************" << endl;

	cout << fixed;
	cout.precision(5);
	outp << fixed;
	outp.precision(5);

	bool tflag = 0;
	int count;
	double weigh;

	count = CriBank.GetSize();
	cout << " Inactive Cycle Start" << endl;
	for (i = 0; i < input.inactCycle; i++) {
		cout << i + 1 << " inactive cycle";
		outp << i + 1 << " inactive cycle";
		weigh = (double)input.particle_num / (double)count;

		for (j = 0; j < count; j++) {
			CriBank.pop(Data);
			RUN.simulateParticle(input, input.pin, multiCX, Data, CriBank, weigh, tall, tflag);
		}
		count = CriBank.GetSize();

		tall.Tal_keff(input, outp, count, tflag, input.pin, i);
	}
	tall.printTal(input, outp, tflag, input.pin);

	tflag = 1;
	count = CriBank.GetSize();
	cout << " Active Cycle Start" << endl;
	for (i = 0; i < input.actCycle; i++) {
		cout << i + 1 << " active cycle";
		outp << i + 1 << " active cycle";

		weigh = (double)input.particle_num / (double)count;

		for (j = 0; j < count; j++) {
			CriBank.pop(Data);
			RUN.simulateParticle(input, input.pin, multiCX, Data, CriBank, weigh, tall, tflag);
		}
		count = CriBank.GetSize();

		tall.Tal_keff(input, outp, count, tflag, input.pin, i);
	}
	tall.printTal(input, outp, tflag, input.pin);

	
}

void mcman::alpha_eigenMode(readINPUT &input, readCX *multiCX, ptcRUN &RUN) {
	int i, j;
	CriData Data;

	outp.open(outputName);

	input.writeINPUT(outp);
	outp << "******************************EigenMode******************************" << endl;
	cout << "******************************EigenMode******************************" << endl;

	cout << fixed;
	cout.precision(5);
	outp << fixed;
	outp.precision(5);

	tall.pre_alpha = inn.pre_alpha;

	bool tflag = 0;
	int count;
	double weigh;

	count = CriBank.GetSize();
	cout << " Inactive Cycle Start" << endl;
	for (i = 0; i < input.inactCycle; i++) {
		cout << i + 1 << " inactive cycle";
		outp << i + 1 << " inactive cycle";
		weigh = (double)input.particle_num / (double)count;

		for (j = 0; j < count; j++) {
			CriBank.pop(Data);
			Data.weight = weigh;
			RUN.alpha_simulateParticle(input, input.pin, multiCX, Data, CriBank, FissionBank, Data.weight, tall, tflag);
		}
		while (FissionBank.pop(Data))
		{
			RUN.alpha_simulateParticle(input, input.pin, multiCX, Data, CriBank, FissionBank, Data.weight, tall, tflag);
		}
		count = CriBank.GetSize();
		//debug_out << endl;

		tall.alpha_Tal_keff(input, outp, count, tflag, input.pin, i);
	}
	tall.alpha_printTal(input, outp, tflag, input.pin);

	tflag = 1;
	count = CriBank.GetSize();
	cout << " Active Cycle Start" << endl;
	for (i = 0; i < input.actCycle; i++) {
		cout << i + 1 << " active cycle";
		outp << i + 1 << " active cycle";
		weigh = (double)input.particle_num / (double)count;

		for (j = 0; j < count; j++) {
			CriBank.pop(Data);
			Data.weight = weigh;
			RUN.alpha_simulateParticle(input, input.pin, multiCX, Data, CriBank, FissionBank, Data.weight, tall, tflag);
		}
		while (FissionBank.pop(Data))
		{
			RUN.alpha_simulateParticle(input, input.pin, multiCX, Data, CriBank, FissionBank, Data.weight, tall, tflag);
		}
		count = CriBank.GetSize();
		//debug_out << endl;

		tall.alpha_Tal_keff(input, outp, count, tflag, input.pin, i);
	}
	tall.alpha_printTal(input, outp, tflag, input.pin);

}