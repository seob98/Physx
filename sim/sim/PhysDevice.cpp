#include "PhysDevice.h"

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
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_Allocator, m_ErrorCallback);

	m_Pvd = PxCreatePvd(*m_Foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, PxTolerancesScale(), true, m_Pvd);

	PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	m_Dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	m_Scene = m_Physics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*m_Physics, PxPlane(0, 1, 0, 0), *m_Material);
	m_Scene->addActor(*groundPlane);
}

void PhysDevice::StepSim()
{
	m_Scene->simulate(1.0f / PX_SIM_FRAMECNT);
	m_Scene->fetchResults(true);
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

				body->setSleepThreshold(PxReal(5.f));						//Sleep ���� ��ȯ�� ���� �Ӱ谪 ����
				body->setWakeCounter(PxReal(5.f));							//Wake ���� ��ȯ�� ���� �Ӱ谪 ����

				body->attachShape(*shape);
				PxRigidBodyExt::updateMassAndInertia(*body, 100.0f);		//dynamic actor ���� ��꿡 �ʿ��� ��� : ����, ������, �����߽�(������ ��ġ ����)
				m_Scene->addActor(*body);									//updateMassAndInertia ���� ����� �Լ��� ����ϸ� dynamic actor�� ��������� ������ �� �ִ�.


			}
			else
			{
				PxRigidStatic* body = m_Physics->createRigidStatic(t.transform(localTm));
				body->attachShape(*shape);
				m_Scene->addActor(*body);
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

		body->setSleepThreshold(PxReal(5.f));						//Sleep ���� ��ȯ�� ���� �Ӱ谪 ����
		body->setWakeCounter(PxReal(5.f));							//Wake ���� ��ȯ�� ���� �Ӱ谪 ����

		body->attachShape(*shape);
		PxRigidBodyExt::updateMassAndInertia(*body, 100.0f);		//dynamic actor ���� ��꿡 �ʿ��� ��� : ����, ������, �����߽�(������ ��ġ ����)
		m_Scene->addActor(*body);									//updateMassAndInertia ���� ����� �Լ��� ����ϸ� dynamic actor�� ��������� ������ �� �ִ�.

		sample2 = body;
	}
	else
	{
		PxRigidStatic* body = m_Physics->createRigidStatic(PxTransform(PxVec3(0.f, 10.f, 0.f)));
		body->attachShape(*shape);
		m_Scene->addActor(*body);

		sample2 = body;
	}
	shape->release();
}

PxRigidDynamic* PhysDevice::CreateHelloWorldDynamic(const PxTransform& t, const PxGeometry& geometry)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*m_Physics, t, geometry, *m_Material, 10.0f);
	dynamic->setAngularDamping(0.00001f);			//ȸ���� ���� ���׷�
	dynamic->setLinearDamping(0.1f);				//�̵��� ���� ���׷�
	m_Scene->addActor(*dynamic);
	
	sample = dynamic;
	return dynamic;
}

void PhysDevice::SetLinearVelocity()
{
	//velocity�� �� ������Ʈ�� �����ϸ� ������ �ӵ��� ��� ���ư���. (����� ������ ���� ���ӵ��� �ش� ������ ����)

	if (sample == NULL)
		return;

	if(InputDevice::GetInstance()->GetKey(Key::Left))
		sample->setLinearVelocity(PxVec3(-10, 0, 0));
	if (InputDevice::GetInstance()->GetKeyDown(Key::Right))
		sample->setLinearVelocity(PxVec3(20, 0, 0));
	if (InputDevice::GetInstance()->GetKeyDown(Key::Down))
		sample->setLinearVelocity(PxVec3(0, 0, 20));
	if (InputDevice::GetInstance()->GetKeyDown(Key::Up))
		sample->setLinearVelocity(PxVec3(0, 0, -20));

	if (InputDevice::GetInstance()->GetKeyDown(Key::Space))
		sample->setLinearVelocity(PxVec3(0, 20, 0));

}

void PhysDevice::SetGlobalPosePosition()
{
	//if (InputDevice::GetInstance()->GetKeyDown(Key::Left))
	//{
	//	PxTransform t = sample->getGlobalPose();
	//	t.p.x += 10;
	//	sample->setGlobalPose(t);
	//}
}

void PhysDevice::SetGlobalPoseRotation()	
{
	if (sample == NULL)
		return;

	static float value = 0.f;

	if (InputDevice::GetInstance()->GetKeyDown(Key::F2))
	{
		PxTransform pose = sample->getGlobalPose();

		value += 5.f;
		pose.q = PxQuat(value, PxVec3(0.f, 1.f, 0.f));		//axis�� normalized �� ��

		sample->setGlobalPose(pose);
	}
}

void PhysDevice::SetKinematicTarget()
{
}

void PhysDevice::AddForce()
{
	//������ �߰�. �̹� �����̰� �ִ� ��ü�� �����ϱ� ����.
	//PxForceMode::eFORCE					���� ����
	//PxForceMode::eACCELERATION			���� ����

	//���������� �ӵ��� ����. ������ ����ϱ� ����
	//PxForceMode::eIMPULSE					���� ����
	//PxForceMode::eVELOCITY_CHANGE			���� ����
	 
	if (sample == NULL)
		return;

	float moveStrength = 10000000.f;
	float jumpStrength = 1500000.f;

	//addTorque : ���ǵ� ���� �������� ������Ʈ�� ȸ��
	if (InputDevice::GetInstance()->GetKey(Key::Left))
		sample->addForce(PxVec3(-moveStrength, 0, 0), PxForceMode::eFORCE);
	if (InputDevice::GetInstance()->GetKey(Key::Right))
		sample->addForce(PxVec3(moveStrength, 0, 0), PxForceMode::eFORCE);
	if (InputDevice::GetInstance()->GetKey(Key::Up))
		sample->addForce(PxVec3(0, 0, moveStrength), PxForceMode::eFORCE);
	if (InputDevice::GetInstance()->GetKey(Key::Down))
		sample->addForce(PxVec3(0, 0, -moveStrength), PxForceMode::eFORCE);

	if (InputDevice::GetInstance()->GetKeyDown(Key::Space))
		sample->addForce(PxVec3(0, jumpStrength, 0), PxForceMode::eIMPULSE);
}

void PhysDevice::RecordStatus()
{
	bool static once{};
	if (!once)
	{
		std::cout << "F1 : status" << std::endl;
		std::cout << "F2 : rotate" << std::endl;
		once = true;
	}
	//if (InputDevice::GetInstance()->GetKeyDown(Key::F1))
	//{
	//	PxTransform t = sample->getGlobalPose();
	//	std::cout << "dynamic actor status" << std::endl;
	//	std::cout << "p - ( " << t.p.x << " " << t.p.y << " " << t.p.z << " )" << std::endl;
	//	std::cout << "q - ( " << t.q.x << " " << t.q.y << " " << t.q.z << " )" << std::endl;
	//	std::cout << "Angle : " << t.q.getAngle() << std::endl;
	//}
}

void PhysDevice::SampleUpdate()
{
	RecordStatus();
	AddForce();
	SetGlobalPoseRotation();
	SetLinearVelocity();
}



