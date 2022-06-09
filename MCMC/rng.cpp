#include <stdio.h>
#include <cmath>
#include "rng.h"

#define LOW16(p)    ( (unsigned long)(p) & 0x0000ffff )
#define UP16(p)     ( LOW16( (p)>>16 ) )

rng::rng(){
	//l[3] = 0x2545;
	//l[2] = 0xF491;
	//l[1] = 0x4F6C;
	//l[0] = 0xDD1D;
	//xi[3] = 0x0000; xi[2] = 0x1234; xi[1] = 0xabcd; xi[0] = 0x330e;
	////xi[2] = 0x0000; xi[1] = 0x0000; xi[0] = 0x0005;
	//two_m16 = 1.0 / pow(2.0, 16);
	two_m16 = 1. / (1L << 16);
	mult[0] = 0xe66d;    mult[1] = 0xdeec;    mult[2] = 0x0005;
	add = 0x000b;
	rn[0] = 0x330e;      rn[1] = 0xabcd;      rn[2] = 0x1234;
	srn[0] = 0x330e;     srn[1] = 0xabcd;     srn[2] = 0x1234;

}
rng::~rng(){
	
}

double rng::gnRN(){
	
	/*unsigned short bl[12];
	unsigned short bu[8];

	bl[0] = LOW16(l[0] * xi[0]);
	bu[0] = UP16(l[0] * xi[0]);

	bl[1] = LOW16(l[1] * xi[0]);
	bu[1] = UP16(l[1] * xi[0]);

	bl[2] = LOW16(l[0] * xi[1]);
	bu[2] = UP16(l[0] * xi[1]);

	bl[3] = LOW16(l[2] * xi[0]);
	bu[3] = UP16(l[2] * xi[0]);

	bl[4] = LOW16(l[1] * xi[1]);
	bu[4] = UP16(l[1] * xi[1]);

	bl[5] = LOW16(l[0] * xi[2]);
	bu[5] = UP16(l[0] * xi[2]);

	bl[6] = LOW16(bu[0] + bl[1] + bl[2]);
	bu[6] = UP16(bu[0] + bl[1] + bl[2]);

	bl[7] = LOW16(bu[1] + bu[2] + bl[3] + bl[4] + bl[5]);
	bu[7] = UP16(bu[1] + bu[2] + bl[3] + bl[4] + bl[5]);

	bl[8] = LOW16(l[3] * xi[0]);
	bl[9] = LOW16(l[2] * xi[1]);
	bl[10] = LOW16(l[1] * xi[2]);
	bl[11] = LOW16(l[0] * xi[3]);


	xi[0] = bl[0];
	xi[1] = bl[6];
	xi[2] = bu[6] + bl[7];
	xi[3] = bu[7] + bu[3] + bu[4] + bu[5] + bl[8] + bl[9] + bl[10] + bl[11];

	return xi[0] * two_m16*two_m16*two_m16*two_m16 + xi[1] * two_m16*two_m16*two_m16 + xi[2] * two_m16*two_m16 + xi[3] * two_m16;*/

	unsigned long p;
	unsigned short a_L[6], a_U[6];

	p = (unsigned long)mult[0] * (unsigned long)rn[0];
	a_U[0] = (unsigned short)UP16(p); a_L[0] = (unsigned short)LOW16(p);
	p = (unsigned long)a_L[0] + (unsigned long)add;
	a_U[1] = (unsigned short)UP16(p); a_L[1] = (unsigned short)LOW16(p);
	p = (unsigned long)a_U[0] + (unsigned long)a_U[1];
	a_U[2] = (unsigned short)UP16(p); a_L[2] = (unsigned short)LOW16(p);
	p = (unsigned long)mult[1] * (unsigned long)rn[0];
	a_U[3] = (unsigned short)UP16(p); a_L[3] = (unsigned short)LOW16(p);
	p = (unsigned long)mult[0] * (unsigned long)rn[1];
	a_U[4] = (unsigned short)UP16(p); a_L[4] = (unsigned short)LOW16(p);
	p = (unsigned long)a_L[2] + (unsigned long)a_L[3] + (unsigned long)a_L[4];
	a_U[5] = (unsigned short)UP16(p); a_L[5] = (unsigned short)LOW16(p);

	rn[2] = a_U[2] + a_U[3] + a_U[4] + a_U[5]
		+ mult[2] * rn[0] + mult[1] * rn[1] + mult[0] * rn[2];
	rn[1] = a_L[5];
	rn[0] = a_L[1];

	return (two_m16 *(two_m16*(two_m16*rn[0] + rn[1]) + rn[2]));

}