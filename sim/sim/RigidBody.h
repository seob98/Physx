#pragma once

#include "Client_Defines.h"

//enum class Interpolate
//{
//	None,
//	Interpolate,
//	Extrapolate,
//};
//
//enum class PhysicsAxis
//{
//	NONE = 0,			//0000
//	X = 1 << 0,			//0001
//	Y = 1 << 1,			//0010
//	Z = 1 << 2,			//0100
//	All = X | Y | Z,	//0111
//						//AND, OR�������� Ȯ�� ����
//};
//
//enum class ForceMode
//{
//	//������ �߰�. �̹� �����̰� �ִ� ��ü�� �����ϱ� ����.
//	//PxForceMode::eFORCE					���� ����
//	//PxForceMode::eACCELERATION			���� ����
//
//	//���������� �ӵ��� ����. ������ ����ϱ� ����
//	//PxForceMode::eIMPULSE					���� ����
//	//PxForceMode::eVELOCITY_CHANGE			���� ����
//
//	Impulse = PxForceMode::eIMPULSE,
//	Force = PxForceMode::eFORCE,
//	Acceleration = PxForceMode::eACCELERATION,
//	Velocity = PxForceMode::eVELOCITY_CHANGE,
//};

enum class Interpolate;
enum class PhysicsAxis;
enum class ForceMode;
enum class ColliderShape;

class RigidBody
{
	friend class Collider;

public:
	void Init(ColliderShape shape);
	void ApplyFlags();
public:
	void AttachAll();
	void DetachAll();
	
	void Attach(Collider* collider);
	void Detach(Collider* collider);

public:
	PxRigidDynamic* GetBody();
	Collider*		GetCollider(int index);

public:
	// �����ӿ�ũ �����Ǵ´�� �°� ���� ����
	PxVec3 GetPosition();
	void SetPosition(const float x, const float y, const float z, bool sleep);
	void SetRotation(float degree, PhysicsAxis axis);


	PxVec3 GetVelocity() const;
	void SetVelocity(const PxVec3& velocity);
	float GetMass() const;
	void SetMass(float value);
public:
	void UpdateMassAndInertia();
	bool IsRigidbodySleep() const;
	void SetRigidbodySleep(bool value);
	float GetSleepThresholder() const;
	void SetSleepThresholder(float value);

public:
	void SetRotationLockAxis(PhysicsAxis axes, bool value);
	void SetKinematic(bool value);
	void SetCCDFlag(bool value);
	bool isKinematic() const;

	/*
	//bool IsRigidbodySleep() const;

	//void SetRigidbodySleep(bool value);

	//float GetMass() const;

	//void SetMass(float value);

	//void SetLinearDamping(float value);

	//float GetLinearDamping() const;

	//void SetAngularDamping(float value);

	//float GetAngularDamping() const;

	////Vec3 GetVelocity() const;

	//void SetVelocity(const Vec3& velocity);

	//void AddForce(const Vec3& force, ForceMode forceMode);

	//void ClearForce(ForceMode forceMode);

	////Vec3 GetAngularVelocity() const;

	//void SetAngularVelocity(const Vec3& angularVelocity);

	//void SetRotationLockAxis(PhysicsAxis axes, bool value);

	//bool GetRotationLockAxis(PhysicsAxis axes) const;

	//void SetTranslationLockAxis(PhysicsAxis axes, bool value);

	//bool GetTranslationLockAxis(PhysicsAxis axes) const;
*/

private:
	PxRigidDynamic* m_body;
	bool m_continuous{true};

	vector<Collider*> m_colliders;
};

