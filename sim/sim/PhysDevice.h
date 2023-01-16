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

	void SetLinearVelocity();			//���ӵ� ����
	void SetGlobalPose();				//��ġ ����
	void SetKinematicTarget();
	void AddForce();					//�� ����

	//velocity�� ��� �����ϸ� ������ �ӵ��� ��� ���ư���. (����� ������ ���� ���ӵ��� �ش� ������ ����)
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

