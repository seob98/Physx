#pragma once
#include "Client_Defines.h"

class MySimulationEventCallback : public PxSimulationEventCallback
{
public:
    // Override the other callback methods with empty implementations
    virtual void onConstraintBreak(physx::PxConstraintInfo*, physx::PxU32) { 
        cout << "hi1" << endl; 
    }
    virtual void onWake(physx::PxActor**, physx::PxU32) { 
        cout << "hi2" << endl; 
    }
    virtual void onSleep(physx::PxActor**, physx::PxU32) { 
        cout << "hi3" << endl; }

    virtual void onTrigger(physx::PxTriggerPair*, physx::PxU32) {
        cout << "hi4" << endl; 
    }
    
    // scene->simulate 이후 후처리
    virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) { 
        cout << "hi1" << endl; 
    }
    
    // 충돌시 반환
	virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;

public:
    vector<pair<PxRigidActor*, PxRigidActor*>> m_collidingPairs;
};

