#ifndef _rng_h_
#define _rng_h_
#include <math.h>

class rng{
//public:
//	double two_m16;
//
//	unsigned short l[4];// 2685 8216 5773 6338 717 2685821657736338717 2545 F491 4F6C DD1D
//	unsigned short xi[4];
//
//	rng();
//	~rng();
//
//	double gnRN(void);


protected:
	double two_m16; //2^(-16)
	unsigned short rn[3]; //rn = rn[2]*(2^32) + rn[1]*(2^16) + rn[0]
	unsigned short srn[3]; //base rn for striding, srn = srn[2]*(2^32) + srn[1]*(2^16) + srn[0]
	unsigned short mult[3], add; // RN_NEW = ( RN_PREV * (mult[2]*(2^32) + mult[1]*(2^16) + mult[0] ) + add) mod (2^48)
	unsigned short stride_mult[3], stride_add[3];   //RN(n+strideNum) = ( RN(n) * ( stride_mult[2]*(2^32) 
	//                   + stride_mult[1]*(2^16) + stride_mult[0] )
	//                   + ( stride_add[2]*(2^32) + stride_add[1]*(2^16) 
	//                   + stride_add[0] ) ) mode (2^48)
public:
	rng();
	~rng();


	double gnRN(void);
};
#endif
