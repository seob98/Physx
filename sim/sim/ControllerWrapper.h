#pragma once

#include "Client_Defines.h"

class ControllerWrapper
{
public:
	ControllerWrapper();
	~ControllerWrapper();
public:
	void Init();
	void Jump();
public:
	void SetController(PxCapsuleController* controller);
	PxCapsuleController* GetController();
	void SetSerial(int serial);
	int GetSerial();
	void SetDensity(float density);
	float GetDensity();
	
private:
	PxCapsuleController* m_controller = nullptr;
	int m_serial{};
	float m_density{};
};

