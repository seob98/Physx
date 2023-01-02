#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

int main()
{
	//declare variables
	physx::PxDefaultAllocator		m_DefaultAllocatorCallback;
	physx::PxDefaultErrorCallback	m_DefaultErrorCallback;
	physx::PxDefaultCpuDispatcher* m_Dispatcher = NULL;		//sys
	physx::PxTolerancesScale		m_ToleranceScale;			//colliderProcess
	physx::PxFoundation* m_Foundation = NULL;		//initializeSettings
	physx::PxPhysics* m_Physics = NULL;			//core

	physx::PxScene* m_Scene = NULL;
	physx::PxMaterial* m_Material = NULL;

	physx::PxPvd* m_Pvd = NULL;				//visualDebugger



	// init physx
	m_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_DefaultAllocatorCallback, m_DefaultErrorCallback);
	if (!m_Foundation)
		throw("PxCreateFoundation Fail");

	m_Pvd = PxCreatePvd(*m_Foundation);
	physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);				//pvd È°¼ºÈ­

	m_ToleranceScale.length = 100;
	m_ToleranceScale.speed = 981;
	m_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_Foundation, m_ToleranceScale, true, m_Pvd);

	physx::PxSceneDesc sceneDesc(m_Physics->getTolerancesScale());		//scene init by tolerance setup
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	m_Dispatcher = physx::PxDefaultCpuDispatcherCreate(2);				//numbersOfThread
	sceneDesc.cpuDispatcher = m_Dispatcher;
	sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	m_Scene = m_Physics->createScene(sceneDesc);

	physx::PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
	if (pvdClient)						//pvdClient for scene
	{
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}

	// create sim
	m_Material = m_Physics->createMaterial(0.5f, 0.5f, 0.6f);
	physx::PxRigidStatic* groundPlane = PxCreatePlane(*m_Physics, physx::PxPlane(0, 1, 0, 1), *m_Material);
	m_Scene->addActor(*groundPlane);



	float halfExtent = .5f;
	physx::PxShape* shape = m_Physics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *m_Material);
	physx::PxU32 size = 30;
	physx::PxTransform t{physx::PxVec3(0)};
	for (physx::PxU32 i = 0; i < size; i++)
	{
		for (physx::PxU32 j = 0; j < size - i; j++)
		{
			physx::PxTransform localTm(physx::PxVec3(physx::PxReal(j * 2) - physx::PxReal(size - i), physx::PxReal(i * 2 + 1), 0) * halfExtent);
			physx::PxRigidDynamic* body = m_Physics->createRigidDynamic(t.transform(localTm));
			body->attachShape(*shape);
			physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			m_Scene->addActor(*body);
		}
	}
	shape->release();
	


	// run sim
	while (1)
	{
		m_Scene->simulate(1.0f / 60.0f);
		m_Scene->fetchResults(true);
	}
}