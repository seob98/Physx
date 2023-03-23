#pragma once
#include "Client_Defines.h"

class RigidBody;
class CapsuleCollider;

enum class CollisionInfoType;

class Player
{
public:
	Player();
	~Player();
public:
	void Init();
	void SetRigidBody(RigidBody* body);

public:
	void Update();
	bool CheckOnGround(CollisionInfoType type, PxVec3& surfaceNormal);
	void GetSlidingVector(CollisionInfoType type);
	bool CheckOnGround_Raycast();
	void DirectionInput();
	void GetAdjustedVector(CollisionInfoType type);
	void Move();
private:
	PxVec3	m_moveDirection{ 0.f };
	PxVec3	m_slidingVector{ 0.f };

	bool	m_onGround = false;
	int		m_debugPrint = 0;
	float	m_degreeThreshold = 49.9f;
private:
	RigidBody* m_body = nullptr;
	CapsuleCollider* m_collider = nullptr;
};

