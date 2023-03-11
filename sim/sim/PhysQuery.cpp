#include "framework.h"
#include "PhysQuery.h"

void PhysQuery::Init()
{
	m_scene = PhysDevice::GetInstance()->GetScene();
}

bool PhysQuery::Raycast(const physx::PxVec3& origin, const physx::PxVec3& direction, physx::PxReal distance, physx::PxQueryFilterData filterData, physx::PxRaycastBuffer* hitBuffer)
{
	return m_scene->raycast(origin, direction, distance, *hitBuffer, physx::PxHitFlag::eDEFAULT, filterData);
}

bool PhysQuery::Overlap(const physx::PxGeometry& geometry, const physx::PxTransform& transform, physx::PxQueryFilterData filterData, physx::PxOverlapBuffer* hitBuffer)
{
	return m_scene->overlap(geometry, transform, *hitBuffer, filterData);
}

bool PhysQuery::Sweep(const physx::PxGeometry& geometry, const physx::PxTransform& pose, const physx::PxVec3& unitDir, physx::PxReal distance, physx::PxQueryFilterData filterData, physx::PxSweepBuffer* hitBuffer)
{
	return m_scene->sweep(geometry, pose, unitDir, distance, *hitBuffer, physx::PxHitFlag::eDEFAULT, filterData);
}
