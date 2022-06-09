#include <stdio.h>

#include "cell.h"
#include "particle.h"

cell::cell(){
	/*keff_C = 0; keff_T = 0;;
	keff_Csum = 0; keff_Tsum = 0;
	cell_flux = 0;
	cell_power = 0;*/
	
	cmat_cyl = 0;
	cmat_pinout = 0;
	cell_type;
	zh = 0;
	zl = 0;
	z_ref = 0;
	xh = 0;
	xl = 0;
	yh = 0;
	yl=0 ;
	rx0 = 0;
	ry0 = 0;
	radius=0;

	keff_C = 0;
	keff_T=0;
	keff_Csum = 0;
	keff_Tsum=0;

	cell_flux = 0;
	cell_power=0;
	cell_flux_sum = 0;
	cell_power_sum=0;
	cell_flux_sq = 0;
	cell_power_sq=0;

	volume = 0;

}
cell::~cell(){
	

}
