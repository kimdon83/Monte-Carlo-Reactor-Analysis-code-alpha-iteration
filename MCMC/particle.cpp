#include <stdio.h>
#include <cmath>
#include <iostream>

#include "readINPUT.h"
#include "particle.h"
#include "ptcRUN.h"
#include "rng.h"
#include "readCX.h"
#include "mcman.h"

#include "define.h"

extern rng RN;
using namespace std;

particle::particle(){
	t = 0;
	En = 0;
	v = 0;
	for (int i = 0; i < 3; i++){
		ome[i] = 0;
		r[i] = 0;
	}
	weight = 1;
}
particle::~particle(){
	
}
void particle::isoDirection(){
	//v = sqrt(2 * En_p*Mev / m_neutron)*100;

	//double mu = 2 * RN.gnRN() - 1;
	//double phi = 2 * PI*RN.gnRN();
	//ome[0] = sqrt(1 - mu*mu)*cos(phi);
	//ome[1] = sqrt(1 - mu*mu)*sin(phi);
	//ome[2] = mu;
	
	double temp;
	do{
		ome[0] = 2 * RN.gnRN() - 1;
		ome[1] = 2 * RN.gnRN() - 1;
		ome[2] = 2 * RN.gnRN() - 1;
		temp = ome[0] * ome[0] + ome[1] * ome[1] + ome[2] * ome[2];
	} while (temp>=1.0);
	temp = sqrt(temp);
	ome[0] /=temp;
	ome[1] /= temp;
	ome[2] /= temp;
	

	//r[0] = x_p; r[1] = y_p; r[2] = z_p; En = En_p; t = time;
}