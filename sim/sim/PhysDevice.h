#pragma once
#include "framework.h"

class PhysDevice
{
public:
	DeclareSingletone(PhysDevice);

private:
	PhysDevice();
	~PhysDevice();

public:
	void Init();
	void StepSim();
	void Release();


public:
	PxPhysics* GetPhysics() const;

	PxMaterial* GetDefaultMaterial() const;

public:
	//Test�� �Լ�
	void CreateHelloWorldStack(const PxTransform& t, PxU32 size, PxReal halfExtent, bool attributeStatic);
	void CreateHelloWorldBox(bool attributeStatic);
	PxRigidDynamic* CreateHelloWorldDynamic(const PxTransform& t, const PxGeometry& geometry);

	//Sample�Լ�
	void SetLinearVelocity();			//���ӵ� ����
	void SetGlobalPosePosition();		//��ġ ����
	void SetGlobalPoseRotation();		//���� ����
	void SetKinematicTarget();
	void AddForce();					//�� ����

	void RecordStatus();
	void SampleUpdate();

private:
	PxDefaultAllocator		m_Allocator;
	PxDefaultErrorCallback	m_ErrorCallback;

	PxFoundation* m_Foundation = NULL;
	PxPhysics* m_Physics = NULL;

	PxDefaultCpuDispatcher* m_Dispatcher = NULL;
	PxScene* m_Scene = NULL;

	PxMaterial* m_Material = NULL;

	PxPvd* m_Pvd = NULL;


	//=============================
	PxRigidDynamic* sample = NULL;
	PxRigidActor* sample2 = NULL;
};

