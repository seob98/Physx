#include "framework.h"
#include "Player.h"
#include "CapsuleCollider.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_body = PhysDevice::GetInstance()->CreateDynamic(ColliderShape::COLLIDER_CAPSULE, 5, 10, -10);
	m_body->SetSleepThresholder(1e-6f);
	m_body->SetRotation(90, PhysicsAxis::Z);			//capsule is laying by default.
	m_body->SetRotationLockAxis(PhysicsAxis::All, true);
	m_body->SetSleepThresholder(0.001f);

	m_collider = dynamic_cast<CapsuleCollider*>(m_body->GetCollider(0));
	m_body->SetKinematic(false);
}

void Player::SetRigidBody(RigidBody* body)
{
	m_body = body;
}

void Player::Update()
{
	m_debugPrint %= 1000;

	DirectionInput();
	Move();

	m_debugPrint += 1;
}

bool Player::CheckOnGround(CollisionInfoType type, PxVec3& surfaceNormal)
{
	m_onGround = false;

	PxVec3 normal;
	PxVec3 up(0.f, 1.f, 0.f);

	const auto info = m_collider->GetCollisionInfo(type);
	int infoCnt = info.size();
	if (infoCnt > 0)
	{
		for (int i = 0; i < infoCnt; ++i)
		{
			for (int j = 0; j < info[i]->GetContactCount(); ++j)
			{
				normal = info[i]->GetContact(j).normal;
				PxVec3 horizontalNormal = PxVec3(normal.x, 0, normal.z).getNormalized();

				float dotProduct = up.dot(normal);
				dotProduct = max(-1.0f, min(1.0f, dotProduct)); // clamp value by -1~1

				float angleRadians = acos(dotProduct);
				float angleDegrees = angleRadians * (180.0f / PxPi);
				if (angleDegrees < m_degreeThreshold)
				{
					//floor, slopes
					surfaceNormal = normal;
					m_onGround = true;
					return true;
				}
				else
				{
					//walls
				}
			}
		}
	}

	return false;
}

void Player::GetSlidingVector(CollisionInfoType type)
{
	/*
	// move() ���ķ� ȣ��Ǿ�� �ϴ� �Լ��Դϴ�.
	PxVec3	dir = m_moveDirection;
	PxVec3	up(0.f, 1.f, 0.f);
	
	PxVec3	currentPos = m_body->GetPosition();
	float	radius = m_collider->GetRadius();
	float	halfHeight = m_collider->GetHalfHeight();
	float	radianThreshold = m_degreeThreshold * (PxPi / 180);
	
	const auto info = m_collider->GetCollisionInfo(type);
	int infoCnt = info.size();
	
	if (infoCnt > 0)
	{
		for (int i = 0; i < infoCnt; ++i)
		{
			for (int j = 0; j < info[i]->GetContactCount(); ++j)
			{
				Contact cont = info[i]->GetContact(j);
				if (acos(cont.normal.dot(up)) <= radianThreshold)
				{
					// ignore floor/slope contacts
					continue;
				}
	
				// calculate slidingVector
				PxVec3 projection = cont.normal * cont.normal.dot(dir);
				PxVec3 slidingVector = dir - projection;
	
				// Accumulate the sliding vector
				m_slidingVector = slidingVector;
			}
		}
	}
	
	m_slidingVector.normalize();
	*/

	PxVec3 dir = m_moveDirection;
	PxVec3 up(0.f, 1.f, 0.f);
	float radianThreshold = m_degreeThreshold * (PxPi / 180);

	const auto info = m_collider->GetCollisionInfo(type);
	int infoCnt = info.size();

	m_slidingVector = PxVec3(0.f, 0.f, 0.f);

	if (infoCnt > 0)
	{
		for (int i = 0; i < infoCnt; ++i)
		{
			for (int j = 0; j < info[i]->GetContactCount(); ++j)
			{
				Contact cont = info[i]->GetContact(j);
				if (acos(cont.normal.dot(up)) <= radianThreshold)
				{
					// ignore floor/slope contacts
					continue;
				}

				// calculate the direction from the contact point to the character's center
				PxVec3 contactToCenter = m_body->GetPosition() - cont.point;
				contactToCenter.normalize();

				// check if the character is moving away from the wall
				float contactDotProduct = contactToCenter.dot(dir);
				if (contactDotProduct > 0.0f)
				{
					// moving away from the wall, no sliding vector should be applied
					continue;
				}

				// calculate projection of the movement direction onto the surface normal
				PxVec3 projection = cont.normal * cont.normal.dot(dir);

				// calculate the sliding vector
				PxVec3 slidingVector = dir - projection;

				// accumulate the sliding vector
				m_slidingVector += slidingVector;
			}
		}
	}

	if (m_slidingVector.magnitudeSquared() > 0.0f)
	{
		m_slidingVector.normalize();
	}
}

bool Player::CheckOnGround_Raycast()
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
	
	if (InputDevice::GetInstance()->GetKey(Key::Left))
		m_moveDirection.x = -1.f;
	else if (InputDevice::GetInstance()->GetKey(Key::Right))
		m_moveDirection.x = 1.f;
	if (InputDevice::GetInstance()->GetKey(Key::Up))
		m_moveDirection.z = 1.f;
	else if (InputDevice::GetInstance()->GetKey(Key::Down))
		m_moveDirection.z = -1.f;

	m_moveDirection.normalize();
}

void Player::Move()
{
	if (m_body->isKinematic())
		return;

	float jumpSpeed = 10.f;
	float moveSpeed = 1.f;

	//check onGround

	GetSlidingVector(CollisionInfoType::Stay);
	PxVec3 a{ 0.f };
	CheckOnGround(CollisionInfoType::Stay, a);
	//PxVec3 adjustedVec = GetAdjustedVector(m_slidingVector ,CollisionInfoType::Stay, moveSpeed);

	if (InputDevice::GetInstance()->GetKey(Key::Space) && m_onGround)
	{
		PxVec3 up{ 0.f, 1.f, 0.f };
		m_body->GetPosition() += up * 0.05f;

		//add velocity
		PxVec3 velocity = m_body->GetVelocity();
		velocity.y = jumpSpeed;
		m_body->SetVelocity(velocity);
		//or add force
		//PxVec3 force = m_moveDirection * m_body->GetMass() * deltaTime * adjustmentValue;
		//m_body->GetBody()->addForce(force, PxForceMode::eIMPULSE);

		m_onGround = false;
	}

	//friction adjustment
	if (!m_onGround)
	{
		m_collider->SetFriction(0.f);
	}
	else
	{
		m_collider->SetFriction(1.f);
	}

	// Apply the adjusted vector to the character's velocity
	//m_.y = m_body->GetVelocity().y;
	if (m_slidingVector.magnitude() > 0)
	{
		m_slidingVector.y = m_body->GetVelocity().y;
		m_body->SetVelocity(m_slidingVector);
	}
	else
	{
		m_moveDirection.y = m_body->GetVelocity().y;
		m_body->SetVelocity(m_moveDirection);
	}


	m_slidingVector = PxVec3(0.f);
	m_moveDirection = PxVec3(0.f);
	//m_onGround = false;
}
