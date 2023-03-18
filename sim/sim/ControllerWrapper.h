#pragma once

#include "Client_Defines.h"

enum class PhysicsAxis;

class ControllerWrapper
{
public:
	ControllerWrapper();
	~ControllerWrapper();
public:
	void Init();
	void Update();
public:
	void MoveSample();
	void MoveByKinematic();
	void MoveByNonKinematic();

	void SlopeMovements();
	void Jump();
	bool UpdateOnGround(PxRaycastBuffer* buffer);
	void UpdateKinematic();
public:
	void SetController(PxCapsuleController* controller);
	PxCapsuleController* GetController();
	void SetSerial(int serial);
	int GetSerial();
	void SetDensity(float density);
	float GetDensity();
public:
	void SetRotationLockAxis(PhysicsAxis axes, bool value);
	bool IsKinematic() const;
	void SetKinematic(bool value);

private:
	PxCapsuleController*	m_controller = nullptr;
	PxRigidDynamic*			m_body = nullptr;

	float	m_radius{};
	float	m_halfHeight{};

	int		m_serial{};
	float	m_density{};
};

