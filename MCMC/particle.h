#ifndef _particle_h_
#define _particle_h_

#include "rng.h"
#include "define.h"
#include "readCX.h"
#include "readINPUT.h"

struct CriData {
	double x, y, z, En;
	double time;
	double weight;
	double ome[3];
	int Grp;
	
};

struct queue {
	CriData Data;
	struct queue *next;
};
typedef struct queue CriticalBankData;

class CriticalBank {
private:
	CriticalBankData *Front, *Rear;
	int size;

public:
	CriticalBank() {
		size = 0;
		Front = Rear = NULL;
	}
	~CriticalBank() {
		CriData data;
		while (pop(data));
	}

	void push(CriData cData) {
		CriticalBankData *temp = new CriticalBankData;
		temp->Data = cData;
		temp->next = NULL;

		if (Front == NULL) Front = temp;
		else Rear->next = temp;

		Rear = temp;
		size++;
	}
	CriData front() {
		return Front->Data;
	}
	bool pop(CriData &Data) {
		if (size == 0) return FALSE;

		CriticalBankData *temp = Front;
		Data = Front->Data;
		Front = Front->next;
		size--;
		delete temp;
		return TRUE;
	}

	int GetSize() { return size; }
};

class particle{
public:

	double t, En;
	double v;
	double ome[3],r[3];
	double weight;
	int grp;


	particle();
	~particle();

	void isoDirection();

	
};
#endif