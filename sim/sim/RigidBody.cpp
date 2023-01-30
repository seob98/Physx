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

	std::vector<Collider*> colliders; // gameObj->comp(�ݶ��̴�);
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

void RigidBody::SetPosition(const float x, const float y, const float z)
{
}

void RigidBody::SetRotation(float radian, PxVec3 axis)
{
}

void RigidBody::UpdateMassAndInertia()
{
	PxRigidBodyExt::setMassAndUpdateInertia(*m_body, m_body->getMass());
}
