#pragma once
#include "Client_Defines.h"

class PhysQuery
{
public:
    void Init();

public:
    bool Raycast(const physx::PxVec3& origin, const physx::PxVec3& direction, physx::PxReal distance, physx::PxQueryFilterData filterData, physx::PxRaycastBuffer* hitBuffer);
    bool Overlap(const physx::PxGeometry& geometry, const physx::PxTransform& transform, physx::PxQueryFilterData filterData, physx::PxOverlapBuffer* hitBuffer);
    bool Sweep(const physx::PxGeometry& geometry, const physx::PxTransform& pose, const physx::PxVec3& unitDir, physx::PxReal distance, physx::PxQueryFilterData filterData, physx::PxSweepBuffer* hitBuffer);

private:
    physx::PxScene* m_scene;
};

