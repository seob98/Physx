#pragma once

#include "Client_Defines.h"

class RigidBody;
enum class PhysicsQueryType;

class CustomQueryFilterCallback : public PxQueryFilterCallback {
public:
	// Set all to target
	CustomQueryFilterCallback(PhysicsQueryType queryType, bool queryOnce, class RigidBody* ignoreBody = nullptr);

	// Set specific
	CustomQueryFilterCallback(PxU32 targetLayerBits, PhysicsQueryType queryType, bool queryOnce, class RigidBody* ignoreBody = nullptr);

	virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData,
		const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) override;

    PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit) override;

private:
	PxU32					m_targets{};
	RigidBody*				m_ignoreBody{};
	PhysicsQueryType		m_queryType{};
	PxQueryHitType::Enum	m_hitType{};
};
