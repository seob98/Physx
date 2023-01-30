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

	//Sample�Լ�
	void SetLinearVelocity();			//���ӵ� ����
	void SetGlobalPosePosition();		//��ġ ����
	void SetGlobalPoseRotation();		//���� ����
	void SetKinematicTarget();
	void AddForce();					//�� ����

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

