#include "fspch.h"
#include "CollisionSystem.h"

#include "Fengshui/Physics/Physics/Contact.h"
#include "Fengshui/Physics/Physics/Intersections.h"
#include "Fengshui/Physics/Physics/Broadphase.h"
#include "Fengshui/Physics/Physics/Shapes.h"


namespace Fengshui
{
	int CompareContacts(const void* p1, const void* p2) {
		contact_t a = *(contact_t*)p1;
		contact_t b = *(contact_t*)p2;

		if (a.timeOfImpact < b.timeOfImpact) {
			return -1;
		}

		if (a.timeOfImpact == b.timeOfImpact) {
			return 0;
		}
		return 1;
	}

	void CollisionSystem::OnUpdate(float dt)
	{
		//m_manifolds.RemoveExpired();

		//Broadphase to retain only the possible collsions to reduce collision calculation in the narrow phase
		std::vector< collisionPair_t > collisionPairs;
		BroadPhase(m_Entities, (int)m_Entities.size(), collisionPairs, dt);

		//narrow phase, where general contact and collision are calculated
		int numContacts = 0;
		const int maxContacts = (int)m_Entities.size() * (int)m_Entities.size();
		contact_t* contacts = (contact_t*)alloca(sizeof(contact_t) * maxContacts);
		for (int i = 0; i < collisionPairs.size(); i++) {
			const collisionPair_t& pair = collisionPairs[i];

			//skip infinite mass pairs, defer checking to intersect
			//if (0.0f == bodyA->m_invMass && 0.0f == bodyB->m_invMass) {
			//	continue;
			//}

			contact_t contact;
			if (Intersect(pair, dt, contact)) {
				if (contact.timeOfImpact == 0.0f) {
					//m_manifolds.AddContact(contact);
				}
				//else {//use dynamic contact checking
					contacts[numContacts] = contact;
					numContacts++;
				//}
			}
		}

		//sort the times of impact from first to last
		if (numContacts > 1) {
			qsort(contacts, numContacts, sizeof(contact_t), CompareContacts);
		}

		////
		////solve constraints for each constraint
		////
		//for (int i = 0; i < m_constraints.size(); i++) {
		//	m_constraints[i]->PreSolve(dt_sec);
		//}
		//m_manifolds.PreSolve(dt_sec);

		//const int numIter = 5;
		//for (int j = 0; j < numIter; j++) {
		//	for (int i = 0; i < m_constraints.size(); i++) {
		//		m_constraints[i]->Solve();
		//	}
		//	m_manifolds.Solve();
		//}

		//for (int i = 0; i < m_constraints.size(); i++) {
		//	m_constraints[i]->PostSolve();
		//}
		//m_manifolds.PostSolve();

		float accumulatedTime = 0.0f;
		for (int i = 0; i < numContacts; i++) {
			contact_t& contact = contacts[i];
			const float dt_sec = contact.timeOfImpact - accumulatedTime;

			//Update the position up to next collision
			for (EntityID entity : m_Entities) {
				Update(dt_sec, entity);
			}
			ResolveContact(contact);
			accumulatedTime += dt_sec;
		}

		//Update the position for the rest of the frame's time
		const float timeRemaining = dt - accumulatedTime;
		if (timeRemaining > 0.0f) {
			for (EntityID entity : m_Entities) {
				Update(timeRemaining, entity);
			}
		}
	}
}