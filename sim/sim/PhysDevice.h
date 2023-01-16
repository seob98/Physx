#pragma once

#include "PxPhysicsAPI.h"

using namespace physx;

class PhysDevice
{
public:
	void Init();
	void StepSim();
	void Release();

	void CreateStack(const PxTransform& t, PxU32 size, PxReal halfExtent, bool attributeStatic);
	PxRigidDynamic* CreateDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0));

	void SetLinearVelocity();

private:

	PxDefaultAllocator		gAllocator;
	PxDefaultErrorCallback	gErrorCallback;

	PxFoundation* gFoundation = NULL;
	PxPhysics* gPhysics = NULL;

	PxDefaultCpuDispatcher* gDispatcher = NULL;
	PxScene* gScene = NULL;

	PxMaterial* gMaterial = NULL;

	PxPvd* gPvd = NULL;

	PxRigidDynamic* sample = NULL;
};

