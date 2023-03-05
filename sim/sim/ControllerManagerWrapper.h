#pragma once

#include "Client_Defines.h"

class ControllerWrapper;

class ControllerManagerWrapper
{
public:
	void Init();
	void CreateController();
	void UpdateControllers();
public:
	PxControllerManager* GetControllerManager();

private:
	PxControllerManager* m_manager = nullptr;
	vector<ControllerWrapper*> m_controllerWrappers;
	PxScene* m_scene = nullptr;
};

