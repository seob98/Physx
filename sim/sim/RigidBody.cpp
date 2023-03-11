#include "framework.h"
#include "RigidBody.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

void RigidBody::Init(ColliderShape shape)
{
	auto device = PhysDevice::GetInstance();

	PxTransform pose(PxIdentity);

	m_body = device->GetPhysics()->createRigidDynamic(pose);
	m_body->setMass(1);
	m_body->userData = this;

	ApplyFlags();

	device->GetScene()->addActor(*m_body);

	switch (shape)
	{
	case ColliderShape::COLLIDER_BOX:
	{
		BoxCollider* box = new BoxCollider;
		box->Init(this);
		m_colliders.emplace_back(box);
		break;
	}
	case ColliderShape::COLLIDER_SPHERE:
	{
		SphereCollider* sphere = new SphereCollider;
		sphere->Init(this);
		m_colliders.emplace_back(sphere);
		break;
	}	
	}
}

void RigidBody::ApplyFlags()
{
	// 연속 충돌 감지 모드를 설정
	bool continousFlag = m_continuous && !isKinematic();
	m_body->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, continousFlag);

	// 연속 충돌 모드 사용시 마찰을 사용합니다.
	m_body->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD_FRICTION, continousFlag);

	// SceneQuery에 현재 오브젝트의 위치가 아닌 Kinematic Traget Transform을 사용
	m_body->setRigidBodyFlag(PxRigidBodyFlag::eUSE_KINEMATIC_TARGET_FOR_SCENE_QUERIES, true);
}

void RigidBody::AttachAll()
{
	DetachAll();

	for (auto collider : m_colliders)
	{
		Attach(collider);
	}
}

void RigidBody::DetachAll()
{
	PxU32 nb = m_body->getNbShapes();
	PxShape** shapes = new PxShape * [nb];
	m_body->getShapes(shapes, sizeof(PxShape*) * nb);

	for (PxU32 i = 0; i < nb; ++i)
	{
		m_body->detachShape(*shapes[i]);
	}

	SafeDeleteArray(shapes);
}

void RigidBody::Attach(Collider* collider)
{
	if (!collider->m_shape)
		return;

	m_body->attachShape(*(collider->m_shape));
	UpdateMassAndInertia();
}

void RigidBody::Detach(Collider* collider)
{
	if (!collider->m_shape)
		return;

	m_body->detachShape(*(collider->m_shape));
	UpdateMassAndInertia();
}

PxRigidDynamic* RigidBody::GetBody()
{
	return m_body;
}

Collider* RigidBody::GetCollider(int index)
{
	if (index >= m_colliders.size())
	{
		return nullptr;
	}
	else
	{
		return m_colliders[index];
	}
}

void RigidBody::SetPosition(const float x, const float y, const float z, bool sleep)
{
	PxTransform t = m_body->getGlobalPose();
	t.p.x = x;
	t.p.y = y;
	t.p.z = z;

	m_body->setGlobalPose(t, sleep);
}

void RigidBody::SetRotation(float radian, PxVec3 axis)
{
	PxTransform t = m_body->getGlobalPose();

	float value = t.q.getAngle();

	t.q = PxQuat(value + 10, axis);		//axis는 normalized 된 값

	m_body->setGlobalPose(t);
}

void RigidBody::UpdateMassAndInertia()
{
	PxRigidBodyExt::setMassAndUpdateInertia(*m_body, m_body->getMass());
}

void RigidBody::SetRotationLockAxis(PhysicsAxis axes, bool value)
{
	PxU32 flag = (PxU32)axes << 3;
	m_body->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::Enum(flag), value);
}

bool RigidBody::isKinematic() const
{
	return m_body->getRigidBodyFlags().isSet(PxRigidBodyFlag::eKINEMATIC);
}
