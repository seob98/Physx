#pragma once

#include "PxPhysicsAPI.h"

using namespace physx;

class PhysDevice
{
public:
	void Init();
	void StepSim();
	void Release();

	void CreateController();

	void CreateStack(const PxTransform& t, PxU32 size, PxReal halfExtent, bool attributeStatic);
	void CreateBox(bool attributeStatic);
	PxRigidDynamic* CreateDynamic(const PxTransform& t, const PxGeometry& geometry);

	//Sample함수
	void SetLinearVelocity();			//가속도 설정
	void SetGlobalPosePosition();		//위치 변경
	void SetGlobalPoseRotation();		//각도 변경
	void SetKinematicTarget();
	void AddForce();					//힘 적용

	void RecordStatus();
	void SampleUpdate();

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

