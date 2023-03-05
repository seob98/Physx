#pragma once
#include "Client_Defines.h"

class MySimulationEventCallback : public PxSimulationEventCallback
{
public:
    // Override the other callback methods with empty implementations
    virtual void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) { }
    virtual void onWake(physx::PxActor**, physx::PxU32) { }
    virtual void onSleep(physx::PxActor**, physx::PxU32) { }

    virtual void onTrigger(physx::PxTriggerPair*, physx::PxU32) {}
    
    // scene->simulate 이후 후처리
    virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) { }
    
    // 충돌시 반환
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;

public:
    void clearVector();

public:
    vector<pair<PxRigidActor*, PxRigidActor*>> m_collidingPairs;
};

