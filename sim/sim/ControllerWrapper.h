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
	void Move();
	void MoveWithNaturalGrav();
	void Jump();
public:
	void SetController(PxCapsuleController* controller);
	PxCapsuleController* GetController();
	void SetSerial(int serial);
	int GetSerial();
	void SetDensity(float density);
	float GetDensity();
	
public:
	void SetRotationLockAxis(PhysicsAxis axes, bool value);
	bool isKinematic() const;

private:
	PxCapsuleController*	m_controller = nullptr;
	PxRigidDynamic*			m_body = nullptr;
	int m_serial{};
	float m_density{};
};

