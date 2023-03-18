#pragma once

#include "Client_Defines.h"

class PhysicsRay
{
public:
	PhysicsRay();
	PhysicsRay(const PxVec3& point, const PxVec3& direction);
	PhysicsRay(const PxVec3& point, const PxVec3& direction, float distance);

public:
	PxVec3 point;
	PxVec3 direction;
	float distance = FLT_MAX;
};

