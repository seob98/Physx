#pragma once
#include "Client_Defines.h"

class Collider;
class RigidBody;

struct Contact
{
	PxVec3 point;
	PxVec3 normal;
	PxVec3 impulse;
	float seperation;
};

class CollisionPairInfo
{
public:
	CollisionPairInfo(const PxContactPair* pairs, bool inverse);

public:
	Collider* GetToCollider() const;
	Collider* GetFromCollider() const;
	unsigned int GetContactCount() const;
	Contact GetContact(unsigned int index) const;

private:
	Collider* m_to = nullptr;		//충돌당한 콜라이더
	Collider* m_from = nullptr;		//충돌한 콜라이더

	unsigned int m_numContacts = 0;

	std::vector<PxContactPairPoint> m_contacts;
};

