#include "framework.h"
#include "MySimulationEventCallback.h"

void MySimulationEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
    for (PxU32 i = 0; i < nbPairs; i++)
    {
        const PxContactPair& cp = pairs[i];
        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
           auto enterB2A  = make_shared<CollisionPairInfo>(pairs, false);
           auto enterA2B = make_shared<CollisionPairInfo>(pairs, true);
           m_CollisionEnter.push_back(std::move(enterB2A));
           m_CollisionEnter.push_back(std::move(enterA2B));
        }
        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
        {
            auto stayB2A = make_shared<CollisionPairInfo>(pairs, false);
            auto stayA2B = make_shared<CollisionPairInfo>(pairs, true);
            m_CollisionStay.push_back(std::move(stayB2A));
            m_CollisionStay.push_back(std::move(stayA2B));
        }
        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            auto exitB2A = make_shared<CollisionPairInfo>(pairs, false);
            auto exitA2B = make_shared<CollisionPairInfo>(pairs, true);
            m_CollisionExit.push_back(std::move(exitB2A));
            m_CollisionExit.push_back(std::move(exitA2B));
        }
    }
}

void MySimulationEventCallback::Notify()
{
    for (auto& info : m_CollisionEnter)
    {
        Collider* to = info.get()->GetToCollider();
        to->CollectCollisionInfo(EventCallbackInfoType::Enter, info);
    }
    for (auto& info : m_CollisionStay)
    {
        Collider* to = info.get()->GetToCollider();
        to->CollectCollisionInfo(EventCallbackInfoType::Stay, info);
    }
    for (auto& info : m_CollisionExit)
    {
        Collider* to = info.get()->GetToCollider();
        to->CollectCollisionInfo(EventCallbackInfoType::Exit, info);
    }
}

void MySimulationEventCallback::ClearVector()
{
    m_CollisionEnter.clear();
    m_CollisionStay.clear();
    m_CollisionExit.clear();
}
