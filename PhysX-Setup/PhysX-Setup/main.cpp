#include "stadfx.h"

int main()
{
	CPhysics p = CPhysics();

	p.init();
	p.CreatePlane();
	p.CreateStack();
	p.RunSim();
}