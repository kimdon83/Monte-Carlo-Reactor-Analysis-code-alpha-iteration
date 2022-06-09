#include <stdio.h>
#include <iostream>

#include "rng.h"
#include "ptcRUN.h"
#include "particle.h"
#include "cell.h"
#include "readCX.h"
#include "ptcRUN.h"

#include "define.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))  
extern unsigned _int64 csi;
extern rng RN;

extern ofstream debug_out;

using namespace std;

ptcRUN::ptcRUN() {
	matRUN = 1;

}
ptcRUN::~ptcRUN() {


}
int ptcRUN::find_Location(particle &part, readINPUT &input, cell *pin) {


	double x_in, y_in;
	index = (int)(part.r[0] / input.x_len_fi) + ((int)(part.r[1] / input.y_len_fi))*input.x_num_fi;

	x_in = part.r[0] - pin[index].xl;
	y_in = part.r[1] - pin[index].yl;

	cyl_check = (((x_in - pin[index].rx0)*(x_in - pin[index].rx0) + (y_in - pin[index].ry0)*(y_in - pin[index].ry0)) < pin[index].radius*pin[index].radius);

	pinref_check = part.r[2] < pin[index].zh;//1이면 아래, 0이면 ref region

	if (pinref_check) {
		if (cyl_check) {
			matRUN = pin[index].cmat_cyl;
		}
		else {
			matRUN = pin[index].cmat_pinout;
		}
	}
	else {
		matRUN = pin[index].cmat_pinout;
	}

	return index;
}
void ptcRUN::BCtest(particle &part, readINPUT &input, cell *pin) {
	int i;

	// x left right, y bottom top, z bottom top
	outside_check[0] = ((part.r[0] <= outside_location[0]));
	outside_check[1] = ((part.r[0] >= outside_location[1]));
	outside_check[2] = ((part.r[1] <= outside_location[2]));
	outside_check[3] = ((part.r[1] >= outside_location[3]));
	outside_check[4] = ((part.r[2] <= outside_location[4]));
	outside_check[5] = ((part.r[2] >= outside_location[5]));
	outside_check[6] = 0;
	for (i = 0; i < 6; i++) {
		outside_check[6] += outside_check[i];
	}
	if (outside_check[6] != 0) {
		for (i = 0; i < 6; i++) {
			if (outside_check[i] == 1) {
				if (input.bc[i] == reflection) {
					int Num = i / 2;
					part.ome[Num] = -part.ome[Num];
					part.r[Num] = 2 * outside_location[i] - part.r[Num];
				}

				else if (input.bc[i] == zero_flux) {
					part.weight = 0;
					break;
				}

			}
		}
	}

}


void ptcRUN::CAL_outside_location(readINPUT &input, cell *pin) {
	outside_location[0] = pin[0].xl;
	outside_location[1] = pin[input.x_num_fi*input.y_num_fi - 1].xh;
	outside_location[2] = pin[0].yl;
	outside_location[3] = pin[input.x_num_fi*input.y_num_fi - 1].yh;
	outside_location[4] = pin[0].zl;
	outside_location[5] = pin[0].z_ref;
}


double ptcRUN::DTS(particle &part, readINPUT &input, cell *pin) {
	bool cyl_temp = true;
	double distance, dis_cyl, dis_z1, dis_z2, dis_x1, dis_x2, dis_y1, dis_y2, dis_z, dis_x, dis_y;
	double x0, y0, z0, ome[3];
	double a, b, c;//second order equation coefficient
	x0 = part.r[0]; y0 = part.r[1]; z0 = part.r[2]; ome[0] = part.ome[0]; ome[1] = part.ome[1]; ome[2] = part.ome[2];
	int surface[3];
	distance = 0;
	if (pin[index].cell_type == Reflector) {//Reflector Region
		//distance to z_surface
		dis_z1 = abs((pin[index].z_ref - z0) / ome[2]);
		dis_z2 = abs((pin[index].zl - z0) / ome[2]);
		if (((pin[index].z_ref - z0) * ome[2]) > 0) {// direcrion check
			dis_z = dis_z1;
			surface[2] = 5;
		}
		else
		{
			dis_z = dis_z2;
			surface[2] = 4;

		}
		//distance to x_surface
		dis_x1 = abs((pin[index].xh - x0) / ome[0]);
		dis_x2 = abs((pin[index].xl - x0) / ome[0]);
		if (((pin[index].xh - x0) * ome[0]) > 0) {// direcrion check
			dis_x = dis_x1;
			surface[0] = 1;
		}
		else {
			dis_x = dis_x2;
			surface[0] = 0;
		}
		//distance to y_surface
		dis_y1 = abs((pin[index].yh - y0) / ome[1]);
		dis_y2 = abs((pin[index].yl - y0) / ome[1]);
		if ((pin[index].yh - y0) * ome[1] > 0) {// direcrion check
			dis_y = dis_y1;
			surface[1] = 3;
		}
		else {
			dis_y = dis_y2;
			surface[1] = 2;
		}

		//compare & next_surface
		if (dis_x < dis_y) {
			distance = dis_x;
			next_surface = surface[0];
		}
		else {
			distance = dis_y;
			next_surface = surface[1];
		}
		if (dis_z < distance) {
			distance = dis_z;
			next_surface = surface[2];
		}
	}
	else if (pin[index].cell_type == Fuel) {	//Fuel Region
		if (pinref_check) {
			if (cyl_check) {//CYL Region
				// distance to cylinder surface
				double rx, ry;
				rx = pin[index].rx0 + pin[index].xl;
				ry = pin[index].ry0 + pin[index].yl;

				a = ome[0] * ome[0] + ome[1] * ome[1];
				b = 2 * (ome[0] * (x0 - rx) + ome[1] * (y0 - ry));
				c = (x0 - rx)*(x0 - rx) + (y0 - ry)*(y0 - ry) - pin[index].radius*pin[index].radius;
				eq_2nd(a, b, c);
				if (((sol[0] * ome[0] + x0 - rx)*ome[0] + (sol[0] * ome[1] + y0 - ry)*ome[1]) > 0) {
					dis_cyl = abs(sol[0]);
				}
				else dis_cyl = abs(sol[1]);
				//distance z_surface
				dis_z1 = abs((pin[index].zh - z0) / ome[2]);
				dis_z2 = abs((pin[index].zl - z0) / ome[2]);
				if (((pin[index].zh - z0) * ome[2]) > 0) {
					dis_z = dis_z1;
					surface[2] = inpin;
				}
				else {
					dis_z = dis_z2;
					surface[2] = 4;
				}
				// compare dis_z,dis_cyl	//compare & next_surface
				if (dis_z < dis_cyl) {
					distance = dis_z;
					next_surface = surface[2];
				}
				else {
					distance = dis_cyl;
					next_surface = cylSur;
				}
			}
			else if (!cyl_check) {//cyl_OUT Region
				//distance to z_surface
				dis_z1 = abs((pin[index].zh - z0) / ome[2]);
				dis_z2 = abs((pin[index].zl - z0) / ome[2]);
				if (((pin[index].zh - z0) * ome[2]) > 0) {
					dis_z = dis_z1;
					surface[2] = inpin;
				}
				else {
					dis_z = dis_z2;
					surface[2] = 4;
				}
				//distance to x_surface
				dis_x1 = abs((pin[index].xh - x0) / ome[0]);
				dis_x2 = abs((pin[index].xl - x0) / ome[0]);
				if (((pin[index].xh - x0) * ome[0]) > 0) {
					dis_x = dis_x1;
					surface[0] = 1;
				}
				else {
					dis_x = dis_x2;
					surface[0] = 0;
				}
				//distance to y_surface
				dis_y1 = abs((pin[index].yh - y0) / ome[1]);
				dis_y2 = abs((pin[index].yl - y0) / ome[1]);
				if ((pin[index].yh - y0) * ome[1] > 0) {
					dis_y = dis_y1;
					surface[1] = 3;
				}
				else {
					dis_y = dis_y2;
					surface[1] = 2;
				}
				//distance to cyl
				double rx, ry;
				rx = pin[index].rx0 + pin[index].xl;
				ry = pin[index].ry0 + pin[index].yl;

				a = ome[0] * ome[0] + ome[1] * ome[1];
				b = 2 * (ome[0] * (x0 - rx) + ome[1] * (y0 - ry));
				c = (x0 - rx)*(x0 - rx) + (y0 - ry)*(y0 - ry) - pin[index].radius*pin[index].radius;
				eq_2nd(a, b, c);
				if (no_sol == 0) {//dis_cyl exist
					if (b < 0) {
						dis_cyl = sol[1];
					}
					else {
						cyl_temp = 0;
					}

					// compare dis_z,dis_x,dis_y,dis_cyl	//compare & next_surface
					if (dis_x < dis_y) {
						distance = dis_x;
						next_surface = surface[0];
					}
					else {
						distance = dis_y;
						next_surface = surface[1];
					}
					if (dis_z < distance) {
						distance = dis_z;
						next_surface = surface[2];
					}
					if (cyl_temp) {
						if (dis_cyl < distance) {
							distance = dis_cyl;
							next_surface = cylSur;
						}
					}

				}
				else {
					//compare & next_surface
					if (dis_x < dis_y) {
						distance = dis_x;
						next_surface = surface[0];
					}
					else {
						distance = dis_y;
						next_surface = surface[1];
					}
					if (dis_z < distance) {
						distance = dis_z;
						next_surface = surface[2];
					}
				}
			}
		}

		else {
			//distance to z_surface
			dis_z1 = abs((pin[index].z_ref - z0) / ome[2]);
			dis_z2 = abs((pin[index].zh - z0) / ome[2]);
			if (((pin[index].z_ref - z0) * ome[2]) > 0) {// direcrion check
				dis_z = dis_z1;
				surface[2] = 5;
			}
			else {
				dis_z = dis_z2;
				surface[2] = inpin;
			}
			//distance to x_surface
			dis_x1 = abs((pin[index].xh - x0) / ome[0]);
			dis_x2 = abs((pin[index].xl - x0) / ome[0]);
			if (((pin[index].xh - x0) * ome[0]) > 0) {// direcrion check
				dis_x = dis_x1;
				surface[0] = 1;
			}
			else {
				dis_x = dis_x2;
				surface[0] = 0;
			}
			//distance to y_surface
			dis_y1 = abs((pin[index].yh - y0) / ome[1]);
			dis_y2 = abs((pin[index].yl - y0) / ome[1]);
			if ((pin[index].yh - y0) * ome[1] > 0) {// direcrion check
				dis_y = dis_y1;
				surface[1] = 3;
			}
			else {
				dis_y = dis_y2;
				surface[1] = 2;
			}

			//compare & next_surface
			if (dis_x < dis_y) {
				distance = dis_x;
				next_surface = surface[0];
			}
			else {
				distance = dis_y;
				next_surface = surface[1];
			}
			if (dis_z < distance) {
				distance = dis_z;
				next_surface = surface[2];
			}
		}
	}
	return distance;
}

int ptcRUN::find_Group(particle &part, readINPUT &input, cell *pin, readCX *CX) {
	int i;
	for (i = 0; i < 7; i++) {
		if (part.En >= CX[matRUN - 1].en_boundary[i + 1]) {
			EgroupRUN = i;
			break;
		}
	}
	if (part.En < CX[matRUN - 1].en_boundary[7]) {
		EgroupRUN = 6;// energy is bigger than largest energy boundary
	}

	else if (part.En >= CX[matRUN - 1].en_boundary[0]) {
		EgroupRUN = 0;// energy is smaller than smallest energy boundary
	}

	return EgroupRUN;
}

int ptcRUN::colType(readCX *CX) {
	int col;
	double rn_col = RN.gnRN();
	double cdf_col[3];


	cdf_col[0] = CX[matRUN - 1].total_scatter[EgroupRUN];
	cdf_col[1] = CX[matRUN - 1].total_scatter[EgroupRUN] + CX[matRUN - 1].fission[EgroupRUN];

	rn_col *= cdf_col[1];

	if (rn_col < cdf_col[0]) {
		col = elas;
	}
	else {
		col = Fission;
	}


	return col;
}
void ptcRUN::ptcWeight(particle &part, readINPUT &input, cell *pin, readCX *CX) {
	int en_GR = find_Group(part, input, pin, CX);

	part.weight *= 1 - CX[matRUN - 1].fission[en_GR] / CX[matRUN - 1].trans[en_GR];


}

double ptcRUN::fis_En(particle &part, readINPUT &input, cell *pin, readCX *CX) {
	double Energy = 0;
	double cdf = 0.0;//

	double rn_En = RN.gnRN();

	int engr;

	for (int i = 0; i < gr_num; i++) {
		cdf += CX[matRUN - 1].chi[i];
		if (rn_En < cdf) {
			engr = i;
			part.grp = i;
			EgroupRUN = i;
			Energy = CX[matRUN - 1].en_boundary[i] + RN.gnRN()*(CX[matRUN - 1].en_boundary[i + 1] - CX[matRUN - 1].en_boundary[i]);
			break;
		}
	}

	return Energy;
}

double ptcRUN::scat_En(particle &part, readINPUT &input, cell *pin, readCX *CX) {
	double Energy = 0;

	double cdf[gr_num];
	double rn_En = RN.gnRN();


	cdf[0] = CX[matRUN - 1].scatter[EgroupRUN][0];
	for (int i = 1; i < gr_num; i++) {
		cdf[i] = cdf[i - 1] + CX[matRUN - 1].scatter[EgroupRUN][i];
	}
	
	rn_En*=cdf[gr_num - 1];
	for (int i = 0; i < gr_num; i++) {
		if (rn_En < cdf[i]) {
			part.grp = i;
			Energy = CX[matRUN - 1].en_boundary[i] + RN.gnRN()*(CX[matRUN - 1].en_boundary[i + 1] - CX[matRUN - 1].en_boundary[i]);
			EgroupRUN = i;
			break;
		}
	}

	return Energy;
}

void ptcRUN::eq_2nd(double a, double b, double c) {

	double det;
	det = b*b - 4 * a*c;
	if (det < 0) {
		no_sol = 1;// det<0. no solution
		//sol = 0;
	}
	else {
		sol[0] = (-b + sqrt(det)) / (2 * a);
		sol[1] = (-b - sqrt(det)) / (2 * a);
		no_sol = 0; // det>=0 eqation is solved.
	}
}
void ptcRUN::simulateParticle(readINPUT &input, cell *pin, readCX *CX, CriData Data, CriticalBank &CriBank, double inweight, tally &tall, bool tflag) {

	int sc = -1;//simulation check
	int colType_i;
	double dts_s, dtc_s;
	int count = 0;
	double rn;

	Data2ptc(Data, ptc);
	ptc.weight = inweight;
	EgroupRUN = ptc.grp;
	do {
		find_Location(ptc, input, pin);

		dtc_s = -(log(RN.gnRN())) / CX[matRUN - 1].trans[EgroupRUN];
		dts_s = DTS(ptc, input, pin);

		if (dtc_s < dts_s) {// collision
			flight(dtc_s, ptc);
			esti_C(ptc, input, pin, CX, tall, ptc.weight);

			if (tflag)updateTally(input, pin, CX, tall, dtc_s, inweight);

			ptc.weight *= (CX[matRUN - 1].total_scatter[EgroupRUN] + CX[matRUN - 1].fission[EgroupRUN]) / CX[matRUN - 1].trans[EgroupRUN];
			//russian rulette
			if (ptc.weight < weight_thr) {
				rn = RN.gnRN();
				if (ptc.weight / weight_out > rn)			ptc.weight = weight_out;
				else {
					sc = End;
					break;//sc=End;
				}
			}

			colType_i = colType(CX);
			if (colType_i == elas) // elastic scattering
			{
				ptc.En = scat_En(ptc, input, pin, CX);
				ptc.isoDirection();
			}
			else if (colType_i == Fission)
			{
				//count = (int)(ptc.weight*CX[matRUN - 1].Nu[EgroupRUN] * CX[matRUN - 1].fission[EgroupRUN] / CX[matRUN - 1].trans[EgroupRUN] / tall.pre_k + RN.gnRN());
				count = (int)(ptc.weight*CX[matRUN - 1].Nu[EgroupRUN] / tall.pre_k + RN.gnRN()); // 이렇게 하면 파티클 수
				//count = (int)(ptc.weight*CX[matRUN - 1].Nu[EgroupRUN]   + RN.gnRN()); // 이렇게 하면 파티클 수가 진동한다.

				//double En_tmp = ptc.En;
				//fission bank data
				for (int i = 0; i < count; i++) {
					ptc.En = fis_En(ptc, input, pin, CX);
					ptc.isoDirection();
					ptc2Data(Data, ptc);
					CriBank.push(Data);
				}
				//ptc.En = En_tmp;
				sc = End;
				break;
			}
			else // capture
			{
				sc = End;
				break;
			}
		}
		else {// next cell

			flight(dts_s + delta, ptc);
			if (tflag)updateTally(input, pin, CX, tall, dts_s, ptc.weight);
			BCtest(ptc, input, pin);

			if (ptc.weight == 0) {
				break;
			}

		}
	} while (sc != End);
}

void ptcRUN::alpha_simulateParticle(readINPUT &input, cell *pin, readCX *CX, CriData Data, CriticalBank &CriBank, CriticalBank &FissionBank, double inweight, tally &tall, bool tflag) {

	int sc = -1;//simulation check
	int colType_i;
	double dts_s, dtc_s;
	int count = 0;
	double rn;

	Data2ptc(Data, ptc);
	//ptc.weight = inweight;
	EgroupRUN = ptc.grp;
	do {
		find_Location(ptc, input, pin);

		dtc_s = -(log(RN.gnRN())) / CX[matRUN - 1].trans[EgroupRUN];
		dts_s = DTS(ptc, input, pin);

		if (dtc_s < dts_s) {// collision
			flight(dtc_s, ptc);
			alpha_esti_C(ptc, input, pin, CX, tall, ptc.weight);
			//debug_out << tall.temp_alpha << "\t";

			if (tflag)updateTally(input, pin, CX, tall, dtc_s, ptc.weight);
			//count = (int)(tall.pre_alpha*ptc.weight* CX[matRUN - 1].inv_velocity[EgroupRUN] / CX[matRUN - 1].trans[EgroupRUN] + RN.gnRN());
			count = (int)(tall.pre_alpha*tall.temp_alpha + RN.gnRN());

			for (int i = 0; i < count; i++){
				ptc2Data(Data, ptc);
				CriBank.push(Data);
			}
			ptc.weight *= (CX[matRUN - 1].total_scatter[EgroupRUN] + CX[matRUN - 1].fission[EgroupRUN]) / CX[matRUN - 1].trans[EgroupRUN];
			//russian rulette
			if (ptc.weight < weight_thr) {
				rn = RN.gnRN();
				if (ptc.weight / weight_out > rn) {
					ptc.weight = weight_out;
				}
				else
				{
					sc = End;
					break;//sc=End;
				}
			}
			colType_i = colType(CX);
			if (colType_i == elas) // elastic scattering
			{
				ptc.En = scat_En(ptc, input, pin, CX);
				ptc.isoDirection();
			}
			else if (colType_i == Fission)
			{
				//count = int(ptc.weight*CX[matRUN - 1].Nu[EgroupRUN] + RN.gnRN());
				count = int(CX[matRUN - 1].Nu[EgroupRUN] + RN.gnRN());
				if (count==0)
				{
					sc = End;
					break;

				}
				else
				{
					for (int i = 0; i < count - 1; i++) {
						ptc.En = fis_En(ptc, input, pin, CX);
						ptc.isoDirection();
						ptc2Data(Data, ptc);
						FissionBank.push(Data);
					}
					ptc.En = fis_En(ptc, input, pin, CX);
					EgroupRUN = ptc.grp;
					ptc.isoDirection();

				}

			}
			else // capture
			{
				sc = End;
				break;
			}
		}
		else {// next cell

			flight(dts_s + delta, ptc);
			if (tflag)updateTally(input, pin, CX, tall, dts_s, ptc.weight);
			BCtest(ptc, input, pin);

			if (ptc.weight == 0) {
				break;
			}

		}
	} while (sc != End);
}


void ptcRUN::updateTally(readINPUT &input, cell *pin, readCX *CX, tally &tall, double distance, double inweight) {
	pin[index].cell_flux_temp = distance*inweight / pin[index].volume;
	pin[index].cell_flux += pin[index].cell_flux_temp;
	pin[index].cell_power_temp = distance*inweight*CX[matRUN - 1].Nu[EgroupRUN] * CX[matRUN - 1].fission[EgroupRUN] / pin[index].volume;
	pin[index].cell_power += pin[index].cell_power_temp;


}
void ptcRUN::alpha_esti_C(particle &part, readINPUT &input, cell *pin, readCX *CX, tally &tal, double inweight) {
	//tal.esti_C_temp = inweight*CX[matRUN - 1].Nu[EgroupRUN] * CX[matRUN - 1].fission[EgroupRUN] / CX[matRUN - 1].trans[EgroupRUN];
	//tal.temp_alpha = inweight* CX[matRUN - 1].inv_velocity[EgroupRUN] / CX[matRUN - 1].trans[EgroupRUN];
	tal.temp_alpha = ptc.weight* CX[matRUN - 1].inv_velocity[EgroupRUN] / CX[matRUN - 1].trans[EgroupRUN];
	tal.g_alpha += tal.temp_alpha;
	

}
void ptcRUN::esti_C(particle &part, readINPUT &input, cell *pin, readCX *CX, tally &tal, double inweight) {
	tal.esti_C_temp = inweight*CX[matRUN - 1].Nu[EgroupRUN] * CX[matRUN - 1].fission[EgroupRUN] / CX[matRUN - 1].trans[EgroupRUN];
	tal.esti_C += tal.esti_C_temp;

	//tal.temp_alpha = inweight* CX[matRUN - 1].inv_velocity[EgroupRUN] / CX[matRUN - 1].trans[EgroupRUN];
	//tal.g_alpha += tal.temp_alpha;

}
void ptcRUN::esti_T(particle &part, readINPUT &input, cell *pin, readCX *CX, tally &tal, double inweight, double distance) {
	tal.esti_T_temp = inweight*CX[matRUN - 1].Nu[EgroupRUN] * CX[matRUN - 1].fission[EgroupRUN] * distance;
	tal.esti_T += tal.esti_T_temp;

}

void ptcRUN::flight(double distance, particle &part) {
	part.r[0] += distance*part.ome[0];
	part.r[1] += distance*part.ome[1];
	part.r[2] += distance*part.ome[2];
	part.t += distance / part.v;
}
void ptcRUN::ptc2Data(CriData &Data, particle part) {
	Data.x = part.r[0];
	Data.y = part.r[1];
	Data.z = part.r[2];
	Data.ome[0] = part.ome[0];
	Data.ome[1] = part.ome[1];
	Data.ome[2] = part.ome[2];
	Data.time = part.t;
	Data.En = part.En;
	Data.Grp = part.grp;
	Data.weight = part.weight;
}

void ptcRUN::Data2ptc(CriData & Data, particle part)
{
	ptc.r[0] = Data.x;
	ptc.r[1] = Data.y;
	ptc.r[2] = Data.z;

	ptc.ome[0] = Data.ome[0];
	ptc.ome[1] = Data.ome[1];
	ptc.ome[2] = Data.ome[2];

	ptc.weight = Data.weight;

	ptc.grp = Data.Grp;
	ptc.t = Data.time;

}
