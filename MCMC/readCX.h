#ifndef _readCX_h_
#define _readCX_h_
#include "define.h"


class readCX{
public:
	
	double trans[nGrp];
	double fission[nGrp];
	double Nu[nGrp];
	double scatter[nGrp][nGrp]; //a to b
	double chi[nGrp];
	double inv_velocity[nGrp];

	double total_scatter[nGrp];	// from scatter[][]
	
	double en_boundary[8]; // ������ ��� ���ְ� �׷����� ��������� �����ش�.


	//numbering is g-1 eg. CX[4]-> group 5

	readCX();
	~readCX();
	void read(int);
};
#endif
