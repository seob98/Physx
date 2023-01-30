#pragma once

#include "framework.h"

enum class ForceMode
{
	//������ �߰�. �̹� �����̰� �ִ� ��ü�� �����ϱ� ����.
	//PxForceMode::eFORCE					���� ����
	//PxForceMode::eACCELERATION			���� ����

	//���������� �ӵ��� ����. ������ ����ϱ� ����
	//PxForceMode::eIMPULSE					���� ����
	//PxForceMode::eVELOCITY_CHANGE			���� ����

	Impulse = PxForceMode::eIMPULSE,
	Force = PxForceMode::eFORCE,
	Acceleration = PxForceMode::eACCELERATION,
	Velocity = PxForceMode::eVELOCITY_CHANGE,
};

class RigidBody
{
	friend class Collider;

public:
	void Init();

public:
	void AttachAll();
	void DetachAll();
	
	void Attach(Collider* collider);
	void Detach(Collider* collider);

public:
	// �����ӿ�ũ �����Ǵ´�� �°� ���� ����
	void SetPosition(const float x, const float y, const float z);
	
	//���ʹϾ� ���� �� �۾�
	//void SetRotation(Quat& rotation);

	void SetRotation(float radian, PxVec3 axis);

public:
	void UpdateMassAndInertia();
	bool IsRigidbodySleep() const;

	void SetRigidbodySleep(bool value);

	float GetMass() const;

	void SetMass(float value);

	void SetLinearDamping(float value);

	float GetLinearDamping() const;

	void SetAngularDamping(float value);

	float GetAngularDamping() const;

	Vec3 GetVelocity() const;

	void SetVelocity(const Vec3& velocity);

	void AddForce(const Vec3& force, ForceMode forceMode);

	void ClearForce(ForceMode forceMode);

	Vec3 GetAngularVelocity() const;

	void SetAngularVelocity(const Vec3& angularVelocity);

	void SetRotationLockAxis(PhysicsAxis axes, bool value);

	bool GetRotationLockAxis(PhysicsAxis axes) const;

	void SetTranslationLockAxis(PhysicsAxis axes, bool value);

	bool GetTranslationLockAxis(PhysicsAxis axes) const;

private:
	PxRigidDynamic* m_body;
};

