﻿// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <mutex>
#include <algorithm>
#include <vector>
#include <functional>
#include <utility>
#include <iomanip>
#include <assert.h>
#include <cmath>

using namespace std;

#include <PxPhysicsAPI.h>
//#include "PxPhysicsAPI.h"
using namespace physx;

//#include "Define.h"
//
//#include "InputDevice.h"
//#include "PhysDevice.h"
//
////physX
//#include "Collider.h"
//#include "RigidBody.h"
////#include "CapsuleCollider.h"
//#include "BoxCollider.h"
//#include "SphereCollider.h"
//#include "MySimulationEventCallback.h"







