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
	PxRigidDynamic* CreateDynamic(const PxTransform& t, const PxGeometry& geometry);

	void SetLinearVelocity();			//가속도 설정
	void SetGlobalPose();				//위치 변경
	void SetKinematicTarget();
	void AddForce();					//힘 적용

	//velocity을 계속 적용하면 일정한 속도로 계속 나아간다. (명령을 내리는 순간 가속도를 해당 값으로 설정)
	//

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

