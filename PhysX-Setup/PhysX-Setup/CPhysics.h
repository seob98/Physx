#pragma once

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

class CPhysics
{
public:
	CPhysics();
	~CPhysics();
public:
	void init();

	void CreatePlane();
	void CreateStack();
	void RunSim();

private:
	//declare variables
	physx::PxDefaultAllocator		m_DefaultAllocatorCallback;
	physx::PxDefaultErrorCallback	m_DefaultErrorCallback;
	physx::PxDefaultCpuDispatcher*	m_Dispatcher = NULL;		//sys
	physx::PxTolerancesScale		m_ToleranceScale;			//colliderProcess
	physx::PxFoundation*			m_Foundation = NULL;		//initializeSettings
	physx::PxPhysics*				m_Physics = NULL;			//core

	physx::PxScene*					m_Scene = NULL;
	physx::PxMaterial*				m_Material = NULL;

	physx::PxPvd*					m_Pvd = NULL;				//visualDebugger

private:
	physx::PxRigidDynamic*			m_testObj = NULL;
};

