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
	bool CheckOnGroundByCollisionInfo(CollisionInfoType type);
	bool CheckOnGroundByRaycast();
	void DirectionInput();
	void Move();
private:
	PxVec3 m_moveDirection{ 0.f };
	bool m_onGround = false;
private:
	RigidBody* m_body = nullptr;
	CapsuleCollider* m_collider = nullptr;
};

