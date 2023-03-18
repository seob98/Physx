#pragma once
#include "Client_Defines.h"

class RigidBody;
class CapsuleCollider;

class Player
{
public:
	Player();
	~Player();
public:
	void Init();
	void SetRigidBody(RigidBody* body);

public:
	bool CheckOnGround();
	void Move();
private:
	bool m_onGround = false;
private:
	RigidBody* m_body = nullptr;
	CapsuleCollider* m_collider = nullptr;
};

