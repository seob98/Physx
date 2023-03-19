#include "framework.h"
#include "Player.h"
#include "CapsuleCollider.h"

Player::Player()
{
	//CapsuleCollider* capsule = nullptr;
	//Collider* collider = nullptr;
	//dynamic_cast<CapsuleCollider*>(collider);
}

Player::~Player()
{
}

void Player::Init()
{
	m_body = PhysDevice::GetInstance()->CreateDynamic(ColliderShape::COLLIDER_CAPSULE, -10, 10, -20);

	m_body->SetRotation(90, PhysicsAxis::Z);			//capsule is laying by default.
	m_body->SetRotationLockAxis(PhysicsAxis::All, true);
	m_body->SetSleepThresholder(5.f);

	m_collider = dynamic_cast<CapsuleCollider*>(m_body->GetCollider(0));

	m_body->SetKinematic(false);
}

void Player::SetRigidBody(RigidBody* body)
{
	m_body = body;
}

void Player::Update()
{
	DirectionInput();
	Move();
}

bool Player::CheckOnGroundByCollisionInfo(CollisionInfoType type)
{
	auto device = PhysDevice::GetInstance();
	auto query = device->GetQuery();

	//contactnormal로 각도 측정
	PxVec3 normal;
	PxVec3 up(0.f, 1.f, 0.f);

	const auto collisionEnter = m_collider->GetCollisionInfo(type);
	int size = collisionEnter.size();
	if (size > 0)
	{
		normal = collisionEnter[0]->GetContact(0).normal;
		PxVec3 horizontalNormal = PxVec3(normal.x, 0, normal.z).getNormalized(); 

		float dotProduct = up.dot(normal);
		dotProduct = max(-1.0f, min(1.0f, dotProduct)); // clamp value by -1~1

		float angleRadians = acos(dotProduct);
		float angleDegrees = angleRadians * (180.0f / PxPi);
		if (angleDegrees < 49.9f)
		{
			m_onGround = true;
		}
	}

	return false;
}

bool Player::CheckOnGroundByRaycast()
{
	auto device = PhysDevice::GetInstance();
	auto query = device->GetQuery();

	RaycastHit hit;
	PhysicsRay ray;
	ray.direction = PxVec3(0.f, -1.f, 0.f);
	ray.distance = 0.21f;			//45degree-raycast distance : 0.2
	ray.point = m_body->GetPosition()
		- PxVec3(0.f, (m_collider->GetRadius() + m_collider->GetHalfHeight()), 0.f);

	if (query->Raycast(hit, ray, 1 << (uint8_t)PhysicsLayers::Map, PhysicsQueryType::All, m_body))
	{
		return true;
	}

	return false;
}

void Player::DirectionInput()
{
	m_moveDirection = PxVec3(0.f, 0.f, 0.f);
	
	float moveSpeed = 1.f;
	
	if (InputDevice::GetInstance()->GetKey(Key::Left))
		m_moveDirection.x = -moveSpeed;
	if (InputDevice::GetInstance()->GetKey(Key::Right))
		m_moveDirection.x = moveSpeed;
	if (InputDevice::GetInstance()->GetKey(Key::Up))
		m_moveDirection.z = moveSpeed;
	if (InputDevice::GetInstance()->GetKey(Key::Down))
		m_moveDirection.z = -moveSpeed;
}

void Player::Move()
{
	if (m_body->isKinematic())
		return;

	float jumpSpeed = 10.f;

#pragma region onGround check1 & check2
	CheckOnGroundByCollisionInfo(CollisionInfoType::Stay);		
	if (m_onGround)
	{
		m_onGround = CheckOnGroundByRaycast();			
	}
#pragma endregion			

	if (InputDevice::GetInstance()->GetKey(Key::Space) && m_onGround)
	{
		PxVec3 up{ 0.f, 1.f, 0.f };
		m_body->GetPosition() += up * 0.05f;
		
		//raycast 위에 할지는 나중에 결정


		//add velocity
		PxVec3 velocity = m_body->GetVelocity();
		velocity.y = jumpSpeed;
		m_body->SetVelocity(velocity);

		//or
		//add force
		//PxVec3 force = m_moveDirection * m_body->GetMass() * deltaTime * adjustmentValue;
		//m_body->GetBody()->addForce(force, PxForceMode::eIMPULSE);
	
		m_onGround = false;
	}

	if (!m_onGround)
	{
		m_collider->SetFriction(0.f);
	}
	else
	{
		m_collider->SetFriction(1.f);
	}

	m_moveDirection.normalize();
	


	PxVec3 velocity = m_body->GetVelocity();
	m_moveDirection.y = velocity.y;
	m_body->SetVelocity(m_moveDirection);
	
	// force로 돌리고싶으면 force값에 랜덤하게 값 곱해주면됨.
	//float deltaTime = 1.0f / PX_SIM_FRAMECNT;
	//PxVec3 force = m_moveDirection * m_body->getMass()/10.f * deltaTime;
	//m_body->GetBody()->addForce(force, PxForceMode::eVELOCITY_CHANGE);


	m_moveDirection = PxVec3(0.f);
	m_onGround = false;
}
