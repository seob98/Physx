#include "framework.h"
#include "MySimulationEventCallback.h"

void MySimulationEventCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
    for (PxU32 i = 0; i < nbPairs; i++)
    {
        const PxContactPair& cp = pairs[i];
        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            m_collidingPairs.push_back(make_pair(pairHeader.actors[0], pairHeader.actors[1]));
        }
    }
}

void MySimulationEventCallback::clearVector()
{
    m_collidingPairs.clear();
}
