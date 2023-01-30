#pragma once

#define PX_SIM_FRAMECNT 120.0f

#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

#define DeclareSingletone(Name)																		\
private:																							\
static Name g_instance;																				\
public:																								\
static Name* GetInstance();																			\
private:

#define ImplementSingletone(Name)																	\
Name Name::g_instance;																				\
Name* Name::GetInstance()																			\
{																									\
	return &g_instance;																				\
}