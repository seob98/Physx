#include "framework.h"
#include "MyFilterShader.h"

PxFilterFlags MyFilterShader::pairFound(PxU32 pairID, PxFilterObjectAttributes attributes0, PxFilterData filterData0, const PxActor* a0, const PxShape* s0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, const PxActor* a1, const PxShape* s1, PxPairFlags& pairFlags)
{
	const static PxU32 eNotifyFlags =
		PxPairFlag::eDETECT_CCD_CONTACT |
		PxPairFlag::eNOTIFY_TOUCH_FOUND |
		PxPairFlag::eNOTIFY_TOUCH_LOST |
		PxPairFlag::eNOTIFY_CONTACT_POINTS;

	const static PxPairFlags eTriggerFlags =
		PxPairFlag::eTRIGGER_DEFAULT |
		(PxPairFlag::Enum)eNotifyFlags;

	const static PxPairFlags eContactFlags =
		PxPairFlag::eCONTACT_DEFAULT |
		(PxPairFlag::Enum)eNotifyFlags |
		PxPairFlag::eDETECT_CCD_CONTACT |
		PxPairFlag::eNOTIFY_TOUCH_CCD |
		PxPairFlag::eNOTIFY_TOUCH_PERSISTS;

	pairFlags = eContactFlags;

	return PxFilterFlag::eDEFAULT;
}

void MyFilterShader::pairLost(PxU32 pairID, PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, bool objectRemoved)
{
}

bool MyFilterShader::statusChange(PxU32& pairID, PxPairFlags& pairFlags, PxFilterFlags& filterFlags)
{
	return false;
}

PxFilterFlags MyFilterShader::PxDefaultSimulationFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1, 
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		//트리거 방치
		return PxFilterFlag::eSUPPRESS;
	}



	return PxFilterFlag::eCALLBACK;
}
