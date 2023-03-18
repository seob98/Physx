#pragma once

#include "Client_Defines.h"

class RaycastHit
{
public:
	RaycastHit();
public:
	PxVec3 point;
	PxVec3 normal;
	float distance;
	class Collider* collider = nullptr;
};
