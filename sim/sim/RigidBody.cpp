#include "RigidBody.h"

void RigidBody::Init()
{
	auto device = PhysDevice::GetInstance();

	PxTransform pose(PxIdentity);

	m_body = device->GetPhysics()->createRigidDynamic(pose);
	m_body->setMass(1);
	m_body->userData = this;
}

void RigidBody::AttachAll()
{
	DetachAll();

	std::vector<Collider*> colliders; // gameObj->comp(콜라이더);
	for (auto collider : colliders)
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

	t.q = PxQuat(value + 10, PxVec3(0.f, 1.f, 0.f));		//axis는 normalized 된 값

	m_body->setGlobalPose(t);
}

void RigidBody::UpdateMassAndInertia()
{
	PxRigidBodyExt::setMassAndUpdateInertia(*m_body, m_body->getMass());
}
