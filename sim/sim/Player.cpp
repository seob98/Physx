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

	m_collider = dynamic_cast<CapsuleCollider*>(m_body->GetCollider(0));
}

void Player::SetRigidBody(RigidBody* body)
{
	m_body = body;
}

bool Player::CheckOnGround()
{
	auto device = PhysDevice::GetInstance();
	auto query = device->GetQuery();

	//PhysicsRay ray;
	//ray.point = m_body->GetPosition();
	//ray.point.y -= m_collider->GetHalfHeight() + m_collider->GetRadius();
	//ray.direction = PxVec3(0.f, -1.f, 0.f);
	//ray.distance = 50.f;

	//RaycastHit hit;
	////layer, queryType 현재는 사용X
	//query->Raycast(hit, ray, 1 << (uint8_t)PhysicsLayers::Map, PhysicsQueryType::All, m_body);
	//cout << hit.distance << endl;

	m_collider->

	return false;
}

void Player::Move()
{
	CheckOnGround();

	float moveForce = 10.f;
	float jumpStrength = 3.f;

	PxRigidDynamic* body = m_body->GetBody();

	if (body == nullptr)
		return;

	PxVec3 direction = PxVec3(0.f, 0.f, 0.f);
	if (InputDevice::GetInstance()->GetKey(Key::Left))
		direction.x = -1.f;
	if (InputDevice::GetInstance()->GetKey(Key::Right))
		direction.x = 1.f;
	if (InputDevice::GetInstance()->GetKey(Key::Up))
		direction.z = 1.f;
	if (InputDevice::GetInstance()->GetKey(Key::Down))
		direction.z = -1.f;

	//direction.normalize();
	body->addForce(direction * moveForce, PxForceMode::eACCELERATION);

	if (InputDevice::GetInstance()->GetKeyDown(Key::Space))
		body->addForce(PxVec3(0, jumpStrength * 5, 0), PxForceMode::eIMPULSE);
}
