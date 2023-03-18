#include "framework.h"
#include "PhysicsRay.h"

PhysicsRay::PhysicsRay()
{
}

PhysicsRay::PhysicsRay(const PxVec3& point, const PxVec3& direction)
{
	this->point = point;
	this->direction = direction;
}

PhysicsRay::PhysicsRay(const PxVec3& point, const PxVec3& direction, float distance)
{
	this->point = point;
	this->direction = direction;
	this->distance = distance;
}