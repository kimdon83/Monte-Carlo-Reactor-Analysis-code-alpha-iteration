#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>

#include "readINPUT.h"
#include "tally.h"
#include "cell.h"

using namespace std;

tally::tally() {
	count_scat = 0;
	count_cap = 0;
	count_loss = 0;

	pre_k = 1;
	keff_C = 0;
	keff_T = 0;
	keff_Csum = 0;
	keff_Tsum = 0;
	keff_Csq = 0;
	keff_Tsq = 0;
	esti_C = 0;
	esti_C_temp = 0;
	esti_T = 0;
	esti_T_temp = 0;

	g_alpha = 0;
	temp_alpha = 0;
	pre_alpha = 0;
	alpha = 0;

}
tally::~tally() {

}
void tally::Tal_keff(readINPUT inn, ofstream &outp, int count, bool tflag, cell *pin, int Ncycle) {
	int i, j;
	keff_C = esti_C / (double)inn.particle_num;

	pre_k = keff_C;
	nsrc = Ncycle + 1;
	double Csum = keff_Csum / nsrc;
	double Csq = sqrt((keff_Csq / nsrc - Csum*Csum) / (nsrc - 1));
	double Tsum = keff_Tsum / nsrc;
	double Tsq = sqrt((keff_Tsq / nsrc - Tsum*Tsum) / (nsrc - 1));

	cout << "  keff_C" << " " << keff_C << " (" << Csq << ") " << " " << count << endl;
	outp << "  keff_C" << " " << keff_C << " (" << Csq << ") " << " " << count << endl;

	keff_Csum += keff_C;
	keff_Csq += keff_C*keff_C;

	esti_C = 0;

}
void tally::alpha_Tal_keff(readINPUT inn, ofstream &outp, int count, bool tflag, cell *pin, int Ncycle) {
	int i, j;
	//keff_C = esti_C / (double)inn.particle_num;
	alpha = double(inn.particle_num) / g_alpha;
	keff_C = alpha;

	pre_alpha = alpha;
	
	//pre_k = keff_C;
	
	keff_Csum += keff_C;
	keff_Csq += keff_C*keff_C;

	nsrc = Ncycle + 1;
	double Csum = keff_Csum / nsrc;
	double Csq = sqrt((keff_Csq / nsrc - Csum*Csum) / (nsrc - 1));



//	cout << "  keff_C" << " " << keff_C << " (" << Csq << ") " << " " << count << endl;
//	outp << "  keff_C" << " " << keff_C << " (" << Csq << ") " << " " << count << endl;
	
	cout << "  alpha" << " " << alpha << " (" << Csq << ") " << " " << count << endl;
	outp << "  alpha" << " " << alpha << " (" << Csq << ") " << " " << count << endl;

	g_alpha=0;
	esti_C = 0;

}
void tally::printTal(readINPUT inn, ofstream &outp, bool tflag, cell *pin) {
	int i, j;
	if (tflag) {
		nsrc = inn.actCycle;
	}
	else {
		nsrc = inn.inactCycle;
	}

	keff_Csum /= nsrc;
	keff_Csq = sqrt((keff_Csq / nsrc - keff_Csum*keff_Csum) / (nsrc - 1));
	keff_Tsum /= nsrc;
	keff_Tsq = sqrt((keff_Tsq / nsrc - keff_Tsum*keff_Tsum) / (nsrc - 1));

	cout << " keff_C " << keff_Csum << " " << keff_Csq  << endl;
	outp << " keff_C " << keff_Csum << " " << keff_Csq  << endl;


	keff_Csum = 0;
	keff_Csq = 0;
	outp << endl;


}
void tally::alpha_printTal(readINPUT inn, ofstream &outp, bool tflag, cell *pin) {
	int i, j;
	if (tflag) {
		nsrc = inn.actCycle;
	}
	else {
		nsrc = inn.inactCycle;
	}

	keff_Csum /= nsrc;
	keff_Csq = sqrt((keff_Csq / nsrc - keff_Csum*keff_Csum) / (nsrc - 1));
	keff_Tsum /= nsrc;
	keff_Tsq = sqrt((keff_Tsq / nsrc - keff_Tsum*keff_Tsum) / (nsrc - 1));

	cout << " keff_C " << keff_Csum << " " << keff_Csq << endl;
	outp << " keff_C " << keff_Csum << " " << keff_Csq << endl;


	keff_Csum = 0;
	keff_Csq = 0;
	outp << endl;


}