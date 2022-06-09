#ifndef _cell_h_
#define _cell_h_

class cell{
public:
	
	int cmat_cyl, cmat_pinout ;// for material
	int cell_type;// Fuel : 1 , Reflector : 2
	double zh, zl,z_ref;
	double xh, xl, yh, yl;
	double rx0, ry0, radius;
	
	double keff_C, keff_T;
	double keff_Csum, keff_Tsum;

	double cell_flux,cell_power;
	double cell_flux_temp, cell_power_temp;

	double cell_flux_sum, cell_power_sum;
	double cell_flux_sq, cell_power_sq;

	double volume;

	//int Cell_index[4];//index
	//int BC[6];
	cell();
	~cell();

	

};

#endif
