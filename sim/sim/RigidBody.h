#pragma once

#include "framework.h"

enum class ForceMode
{
	//가속의 추가. 이미 움직이고 있는 물체에 적용하기 적합.
	//PxForceMode::eFORCE					무게 적용
	//PxForceMode::eACCELERATION			무게 무시

	//순간적으로 속도를 적용. 점프에 사용하기 적합
	//PxForceMode::eIMPULSE					무게 적용
	//PxForceMode::eVELOCITY_CHANGE			무게 무시

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
	// 프레임워크 구현되는대로 맞게 인자 수정
	void SetPosition(const float x, const float y, const float z);
	
	//쿼터니언 구현 후 작업
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

