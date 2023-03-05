#include "framework.h"
#include "PhysDevice.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include "MyFilterShader.h"

ImplementSingletone(PhysDevice);

PhysDevice::PhysDevice()
{
}

PhysDevice::~PhysDevice()
{
	Release();
}

void PhysDevice::Init()
{
	m_eventCallback = new MySimulationEventCallback;
	m_filterShader = new MyFilterShader;

	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	m_Pvd = PxCreatePvd(*m_Foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true, m_Pvd);

	PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	m_Dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_Dispatcher;
	
	
	//sceneDesc.filterShader = PxDefaultSimulationFilterShader
	sceneDesc.filterShader = MyFilterShader::PxDefaultSimulationFilterShader;
	sceneDesc.filterCallback = m_filterShader;
	sceneDesc.simulationEventCallback = m_eventCallback;

	m_Scene = m_Physics->createScene(sceneDesc);

#pragma region pvd

	PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

#pragma endregion pvd

	m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);
	PxRigidStatic* groundPlane = PxCreatePlane(*m_Physics, PxPlane(0, 1, 0, 0), *m_Material);
	m_Scene->addActor(*groundPlane);

	m_controllerManagerWrapper = new ControllerManagerWrapper;
	m_controllerManagerWrapper->Init();
}

void PhysDevice::StepSim()
{
	m_Scene->simulate(1.0f / PX_SIM_FRAMECNT);
	m_Scene->fetchResults(true);

	m_eventCallback->clearVector();

}

void PhysDevice::Release()
{
	PX_RELEASE(m_Scene);
	PX_RELEASE(m_Dispatcher);
	PX_RELEASE(m_Physics);
	if (m_Pvd)
	{
		PxPvdTransport* transport = m_Pvd->getTransport();
		m_Pvd->release();	m_Pvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(m_Foundation);

	printf("SnippetHelloWorld done.\n");
}

PxPhysics* PhysDevice::GetPhysics() const
{
	return m_Physics;
}

PxMaterial* PhysDevice::GetDefaultMaterial() const
{
	return m_Material;
}

PxScene* PhysDevice::GetScene() const
{
	return m_Scene;
}

ControllerManagerWrapper* PhysDevice::GetControllerManagerWrapper() const
{
	return m_controllerManagerWrapper;
}


void PhysDevice::CreateHelloWorldStack(const PxTransform& t, PxU32 size, PxReal halfExtent, bool attributeStatic)
{
	PxShape* shape = m_Physics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *m_Material);

	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
		
			if (!attributeStatic)
			{
				PxRigidDynamic* body = m_Physics->createRigidDynamic(t.transform(localTm));

				body->setSleepThreshold(PxReal(5.f));						//Sleep 상태 전환을 위한 임계값 설정
				body->setWakeCounter(PxReal(5.f));							//Wake 상태 전환을 위한 임계값 설정

				body->attachShape(*shape);
				PxRigidBodyExt::updateMassAndInertia(*body, 100.0f);		//dynamic actor 질량 계산에 필요한 요소 : 질량, 관성값, 무게중심(관성축 위치 결정)
				m_Scene->addActor(*body);									//updateMassAndInertia 등의 도우미 함수를 사용하면 dynamic actor의 질량계산을 쉽게할 수 있다.

				rigidDynamics.emplace_back(body);
			}
			else
			{
				PxRigidStatic* body = m_Physics->createRigidStatic(t.transform(localTm));
				body->attachShape(*shape);
				m_Scene->addActor(*body);

				rigidStatics.emplace_back(body);
			}
		}
	}
	shape->release();
}

void PhysDevice::CreateHelloWorldBox(bool attributeStatic)
{
	PxShape* shape = m_Physics->createShape(PxBoxGeometry(5.0f, 5.0f, 5.0f), *m_Material);

	if (!attributeStatic)
	{
		PxRigidDynamic* body = m_Physics->createRigidDynamic(PxTransform(PxVec3(0.f, 10.f, 0.f)));

		body->setSleepThreshold(PxReal(5.f));						//Sleep 상태 전환을 위한 임계값 설정
		body->setWakeCounter(PxReal(5.f));							//Wake 상태 전환을 위한 임계값 설정

		body->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*body, 100.0f);		//dynamic actor 질량 계산에 필요한 요소 : 질량, 관성값, 무게중심(관성축 위치 결정)
		m_Scene->addActor(*body);									//updateMassAndInertia 등의 도우미 함수를 사용하면 dynamic actor의 질량계산을 쉽게할 수 있다.

		rigidDynamics.emplace_back(body);
	}
	else
	{
		PxRigidStatic* body = m_Physics->createRigidStatic(PxTransform(PxVec3(0.f, 10.f, 0.f)));
		body->attachShape(*shape);
		m_Scene->addActor(*body);

		rigidStatics.emplace_back(body);
	}
	shape->release();
}

void PhysDevice::CreateHelloWorldDynamic(const PxTransform& t, const PxGeometry& geometry)
{
	sample = PxCreateDynamic(*m_Physics, t, geometry, *m_Material, 10.0f);
	sample->setAngularDamping(0.00001f);			//회전에 대한 저항력
	sample->setLinearDamping(0.1f);				//이동에 대한 저항력
	m_Scene->addActor(*sample);
	
	rigidDynamics.emplace_back(sample);
}

void PhysDevice::InitialPlacement()
{
	CreateDynamic(ColliderShape::COLLIDER_SPHERE, 20, 20, 20);
	CreateDynamic(ColliderShape::COLLIDER_BOX, 20, 20, 0);

#pragma region Sphere크기 조정
	SphereCollider* sphere = dynamic_cast<SphereCollider*>(m_rigidBodies[0]->GetCollider(0));
	if (sphere == nullptr)
		return;
	sphere->SetRadius(2.f);

	PxRigidBody* body = m_rigidBodies[0]->GetBody();
	body->setAngularDamping(0.00001f);
	body->setLinearDamping(0.15f);
	body->setMass(body->getMass() * 0.20f);
	m_rigidBodies[0]->UpdateMassAndInertia();
#pragma endregion

#pragma region Box크기 변경
	BoxCollider* box = dynamic_cast<BoxCollider*>(m_rigidBodies[1]->GetCollider(0));
	if (box == nullptr)
		return;
	box->SetExtents(box->GetExtentX() * 3.f, box->GetExtentX() * 3.f, box->GetExtentX() * 3.f);
#pragma endregion

	m_controllerManagerWrapper->CreateController();
}

void PhysDevice::CreateDynamic(ColliderShape shape, float posX, float posY, float posZ)
{
	RigidBody* body = new RigidBody();
	body->Init(shape);
	body->SetPosition(posX, posY, posZ, true);
	m_rigidBodies.emplace_back(body);
}

void PhysDevice::SetLinearVelocity()
{
	//velocity을 매 업데이트에 적용하면 일정한 속도로 계속 나아간다. (명령을 내리는 순간 가속도를 해당 값으로 설정)

	PxRigidDynamic* body = m_rigidBodies[0]->GetBody();
	if (body == nullptr)
		return;

	if(InputDevice::GetInstance()->GetKey(Key::Left))
		body->setLinearVelocity(PxVec3(-10, 0, 0));
	if (InputDevice::GetInstance()->GetKeyDown(Key::Right))
		body->setLinearVelocity(PxVec3(20, 0, 0));
	if (InputDevice::GetInstance()->GetKeyDown(Key::Up))
		body->setLinearVelocity(PxVec3(0, 0, 20));
	if (InputDevice::GetInstance()->GetKeyDown(Key::Down))
		body->setLinearVelocity(PxVec3(0, 0, -20));

	if (InputDevice::GetInstance()->GetKeyDown(Key::Space))
		body->setLinearVelocity(PxVec3(0, 20, 0));

}

void PhysDevice::SetGlobalPoseRotation()	
{
	static float value = 0.f;

	if (InputDevice::GetInstance()->GetKeyDown(Key::R))
	{
		PxTransform pose = m_rigidBodies[0]->GetBody()->getGlobalPose();

		value += 5.f;
		pose.q = PxQuat(value, PxVec3(0.f, 1.f, 0.f));		//axis는 normalized 된 값

		m_rigidBodies[0]->GetBody()->setGlobalPose(pose);
	}
}

void PhysDevice::AddForce()
{
	float moveStrength = 0.05f;
	float jumpStrength = 3.f;

	PxRigidDynamic* body = m_rigidBodies[0]->GetBody();
	if (body == nullptr)
		return;

	//addTorque : 정의된 축을 기준으로 오브젝트를 회전
	if (InputDevice::GetInstance()->GetKey(Key::Left))
		body->addForce(PxVec3(-moveStrength, 0, 0), PxForceMode::eFORCE);
	if (InputDevice::GetInstance()->GetKey(Key::Right))
		body->addForce(PxVec3(moveStrength, 0, 0), PxForceMode::eFORCE);
	if (InputDevice::GetInstance()->GetKey(Key::Up))
		body->addForce(PxVec3(0, 0, moveStrength), PxForceMode::eFORCE);
	if (InputDevice::GetInstance()->GetKey(Key::Down))
		body->addForce(PxVec3(0, 0, -moveStrength), PxForceMode::eFORCE);

	if (InputDevice::GetInstance()->GetKeyDown(Key::Space))
		body->addForce(PxVec3(0, jumpStrength, 0), PxForceMode::eIMPULSE);
}

void PhysDevice::SampleUpdate()
{
	SetGlobalPoseRotation();
	AddForce();
	m_controllerManagerWrapper->UpdateControllers();
}


