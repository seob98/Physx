#include "framework.h"
#include "PhysQuery.h"
#include "RaycastHit.h"
#include "PhysicsRay.h"
#include "CustomQueryFilterCallback.h"

const PxHitFlags PhysQuery::bufferFlags =
PxHitFlag::ePOSITION |          // �浹 ��ġ
PxHitFlag::eNORMAL |            // �浹 ���
PxHitFlag::eFACE_INDEX |        // �浹�� �������� �ε���
PxHitFlag::eMESH_BOTH_SIDES;    // �ﰢ���� ���ʿ��� ������ �� �ֽ��ϴ�.

const PxQueryFilterData PhysQuery::defaultFilterData
(
	PxQueryFlag::eDYNAMIC |         // dynamic ���� ����
	PxQueryFlag::eSTATIC |          // static  ���� ����
	PxQueryFlag::ePREFILTER         // preFilter�� ���	
);

const PxQueryFilterData PhysQuery::fastFilterData
(
	PxQueryFlag::eDYNAMIC |         // dynamic ���� ����
	PxQueryFlag::eSTATIC |          // static  ���� ����
	PxQueryFlag::ePREFILTER |       // preFilter�� ���	
	PxQueryFlag::eANY_HIT           // ù ���߿� ����
	// �浹 �����Ͱ� �����ϴ�.
);

void PhysQuery::Init()
{
}

bool PhysQuery::Raycast(RaycastHit& hit, const PhysicsRay& ray, PxU32 layerMask, PhysicsQueryType queryType, RigidBody* ignoreBody)
{
    auto device = PhysDevice::GetInstance();
    auto scene = device->GetScene();

    CustomQueryFilterCallback filterCallback(/*layerMask,*/ queryType, true, ignoreBody);
    PxRaycastBuffer hitBuffer;

    bool result = scene->raycast(ray.point, ray.direction, ray.distance,
        hitBuffer, physx::PxHitFlag::eDEFAULT, defaultFilterData, &filterCallback);

    if (result)
    {
        const PxRaycastHit& pxHit = hitBuffer.getAnyHit(0);
        hit.point = pxHit.position;
        hit.normal = pxHit.normal;
        hit.distance = pxHit.distance;
		void* data = pxHit.shape->userData;
		hit.collider = static_cast<Collider*>(data);
    }

    return result;
}

bool PhysQuery::RaycastOld(const physx::PxVec3& origin, const physx::PxVec3& direction, physx::PxReal distance, physx::PxQueryFilterData filterData, physx::PxRaycastBuffer* hitBuffer)
{
	auto device = PhysDevice::GetInstance();
	auto scene = device->GetScene();
	return scene->raycast(origin, direction, distance, *hitBuffer, physx::PxHitFlag::eDEFAULT, filterData);
}

bool PhysQuery::Overlap(const physx::PxGeometry& geometry, const physx::PxTransform& transform, physx::PxQueryFilterData filterData, physx::PxOverlapBuffer* hitBuffer)
{
	auto device = PhysDevice::GetInstance();
	auto scene = device->GetScene();
	return scene->overlap(geometry, transform, *hitBuffer, filterData);
}

bool PhysQuery::Sweep(const physx::PxGeometry& geometry, const physx::PxTransform& pose, const physx::PxVec3& unitDir, physx::PxReal distance, physx::PxQueryFilterData filterData, physx::PxSweepBuffer* hitBuffer)
{
	auto device = PhysDevice::GetInstance();
	auto scene = device->GetScene();
	return scene->sweep(geometry, pose, unitDir, distance, *hitBuffer, physx::PxHitFlag::eDEFAULT, filterData);
}
