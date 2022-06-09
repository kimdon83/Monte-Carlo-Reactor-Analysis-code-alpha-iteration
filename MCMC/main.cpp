/**********************************************************************************************************************
 * AUTHOR : Dong Hun Kim
 * FINAL UPDATE : 2015. 05. 00.
 * VERSION : 1.0.0.0
 *********************************************************************************************************************/
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

using namespace std;
rng RN;

int main() {
	
	/*double rntest;
	double rntest2;
	while (1){
		rntest = RN.gnRN();
		rntest2 = RN.gnRN();
	}*/
	//double rntest;
	//for (int i = 0; i < 12189; i++)
	//{
	//	rntest = RN.gnRN();
	//}

	mcman mcmanager;
	mcmanager.mcRUN();
	
	
	return 0;
}







