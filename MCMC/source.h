#ifndef _source_h_
#define _source_h_

#include "particle.h"
#include "readINPUT.h"
#include "cell.h"
#include "readCX.h"

class source{
public:
	source();
	~source();
	void mk_source(readINPUT &input, CriData &Data);
};
#endif
