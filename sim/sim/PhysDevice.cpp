#include "PhysDevice.h"
#include "framework.h"

void PhysDevice::Init()
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
	gScene->addActor(*groundPlane);
}

void PhysDevice::StepSim()
{
	gScene->simulate(1.0f / 120.0f);
	gScene->fetchResults(true);
}

void PhysDevice::Release()
{
	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);
	PX_RELEASE(gPhysics);
	if (gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		gPvd->release();	gPvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(gFoundation);

	printf("SnippetHelloWorld done.\n");
}

void PhysDevice::CreateStack(const PxTransform& t, PxU32 size, PxReal halfExtent, bool attributeStatic)
{
	PxShape* shape = gPhysics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
	for (PxU32 i = 0; i < size; i++)
	{
		for (PxU32 j = 0; j < size - i; j++)
		{
			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(size - i), PxReal(i * 2 + 1), 0) * halfExtent);
		
			if (!attributeStatic)
			{
				PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));

				body->setSleepThreshold(PxReal(5.f));						//Sleep 상태 전환을 위한 임계값 설정
				body->setWakeCounter(PxReal(5.f));							//Wake 상태 전환을 위한 임계값 설정

				body->attachShape(*shape);
				PxRigidBodyExt::updateMassAndInertia(*body, 100.0f);		//dynamic actor 질량 계산에 필요한 요소 : 질량, 관성값, 무게중심(관성축 위치 결정)
				gScene->addActor(*body);									//updateMassAndInertia 등의 도우미 함수를 사용하면 dynamic actor의 질량계산을 쉽게할 수 있다.


			}
			else
			{
				PxRigidStatic* body = gPhysics->createRigidStatic(t.transform(localTm));
				body->attachShape(*shape);
				gScene->addActor(*body);
			}
		}
	}
	shape->release();
}

PxRigidDynamic* PhysDevice::CreateDynamic(const PxTransform& t, const PxGeometry& geometry)
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.00001f);			//회전에 대한 저항력
	dynamic->setLinearDamping(0.1f);				//이동에 대한 저항력
	gScene->addActor(*dynamic);
	
	sample = dynamic;
	return dynamic;
}
void PhysDevice::SetLinearVelocity()
{
	if(InputDevice::GetInstance()->GetKey(Key::Left))
		sample->setLinearVelocity(PxVec3(-10, 0, 0));
	if (InputDevice::GetInstance()->GetKeyDown(Key::Right))
		sample->setLinearVelocity(PxVec3(20, 0, 0));
	if (InputDevice::GetInstance()->GetKeyDown(Key::Up))
		sample->setLinearVelocity(PxVec3(0, 0, 20));
	if (InputDevice::GetInstance()->GetKeyDown(Key::Down))
		sample->setLinearVelocity(PxVec3(0, 0, -100));

	if (InputDevice::GetInstance()->GetKeyDown(Key::Space))
		sample->setLinearVelocity(PxVec3(0, 20, 0));

}

void PhysDevice::SetGlobalPose()
{
	if (InputDevice::GetInstance()->GetKeyDown(Key::Left))
	{
		PxTransform t = sample->getGlobalPose();
		t.p.x += 10;
		sample->setGlobalPose(t);
	}
}

void PhysDevice::SetKinematicTarget()
{
}

void PhysDevice::AddForce()
{
	//가속의 추가. 이미 움직이고 있는 물체에 적용하기 적합.
	//PxForceMode::eFORCE					무게 적용
	//PxForceMode::eACCELERATION			무게 무시

	//순간적으로 속도를 적용. 점프에 사용하기 적합
	//PxForceMode::eIMPULSE					무게 적용
	//PxForceMode::eVELOCITY_CHANGE			무게 무시
	 
	float moveStrength = 10000000.f;
	float jumpStrength = 1500000.f;

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



