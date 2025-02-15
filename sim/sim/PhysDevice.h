#pragma once
#include "Client_Defines.h"

enum class ColliderShape;

class RigidBody;
class MySimulationEventCallback;
class MyFilterShader;
class ControllerManagerWrapper;
class PhysQuery;
class Player;

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
	PxPhysics*	GetPhysics() const;
	PxScene*	GetScene() const;
	PhysQuery*	GetQuery() const;
	PxCooking*	GetCooking() const;
	PxMaterial*	GetDefaultMaterial() const;
	ControllerManagerWrapper* GetControllerManagerWrapper() const;

public:
	//Test용 함수
	void CreateHelloWorldStack(const PxTransform& t, PxU32 size, PxReal halfExtent, bool attributeStatic);
	void CreateHelloWorldBox(bool attributeStatic);
	void CreateHelloWorldDynamic(const PxTransform& t, const PxGeometry& geometry);
	void InitialPlacement();

	RigidBody* CreateDynamic(ColliderShape shape, float posX, float posY, float posZ);

	void SetLinearVelocity();			//가속도 설정
	void SetGlobalPoseRotation();		//각도 변경
	void AddForce();					//힘 적용

	void GameLogic();
private:
	PxDefaultAllocator				m_Allocator;
	PxDefaultErrorCallback			m_ErrorCallback;

	PxFoundation*					m_Foundation = NULL;
	PxPhysics*						m_Physics = NULL;

	PxDefaultCpuDispatcher*			m_Dispatcher = NULL;
	PxScene*						m_Scene = NULL;

	PxCooking*						m_cooking = NULL;

	PxMaterial*						m_Material = NULL;

	PxPvd* m_Pvd = NULL;
	MySimulationEventCallback*		m_eventCallback		= nullptr;
	MyFilterShader*					m_filterShader					= nullptr;
	ControllerManagerWrapper*		m_controllerManagerWrapper	= nullptr;
	PhysQuery*						m_query = nullptr;

	vector<RigidBody*>				m_RigidBodies;

	Player*							m_player;
};

