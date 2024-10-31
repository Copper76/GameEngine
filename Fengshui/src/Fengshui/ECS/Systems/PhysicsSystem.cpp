#include "fspch.h"
#include "PhysicsSystem.h"

#include "Fengshui/Physics/Contact.h"
#include "Fengshui/Physics/Intersections.h"
#include "Fengshui/Physics/Broadphase.h"
#include "Fengshui/Physics/Shapes.h"


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

	void PhysicsSystem::OnUpdate(const float dt, Ref<ManifoldCollector> manifolds, std::vector<Ref<Constraint>> constraints)
	{
		manifolds->RemoveExpired();

		//Broadphase to retain only the possible collsions to reduce collision calculation in the narrow phase
		std::vector<CollisionPair> collisionPairs;
		BroadPhase(m_Entities, (int)m_Entities.size(), collisionPairs, dt);//Get components here

		//narrow phase, where general contact and collision are calculated
		int numContacts = 0;
		const int maxContacts = (int)m_Entities.size() * (int)m_Entities.size();
		contact_t* contacts = (contact_t*)alloca(sizeof(contact_t) * maxContacts);;
		for (int i = 0; i < collisionPairs.size(); i++) {
			const CollisionPair& pair = collisionPairs[i];

			contact_t contact;
			if (Intersect(pair, dt, contact)) {//Get components here
				if (contact.timeOfImpact == 0.0f) {
					manifolds->AddContact(contact);
				}
				else {//use dynamic contact checking
					contacts[numContacts] = contact;
					numContacts++;
				}
			}
		}

		//sort the times of impact from first to last
		if (numContacts > 1) {
			qsort(contacts, numContacts, sizeof(contact_t), CompareContacts);
			FS_INFO("Contacting")
		}

		//
		//solve constraints for each constraint
		//
		//Get components for each
		for (int i = 0; i < constraints.size(); i++) {
			constraints[i]->PreSolve(dt);
		}
		manifolds->PreSolve(dt);

		const int numIter = 30;
		for (int j = 0; j < numIter; j++) {
			for (int i = 0; i < constraints.size(); i++) {
				constraints[i]->Solve();
			}
			manifolds->Solve();
		}

		for (int i = 0; i < constraints.size(); i++) {
			constraints[i]->PostSolve();
		}
		manifolds->PostSolve();

		float accumulatedTime = 0.0f;
		for (int i = 0; i < numContacts; i++) {
			contact_t& contact = contacts[i];
			const float dt_sec = contact.timeOfImpact - accumulatedTime;

			//Update the position up to next collision
			for (EntityID entity : m_Entities) {
				Update(dt_sec, entity);//Get components here
			}
			ResolveContact(contact);//Get components here
			FS_INFO("Resolving contact")
			accumulatedTime += dt_sec;
		}

		//Update the position for the rest of the frame's time
		const float timeRemaining = dt - accumulatedTime;
		if (timeRemaining > 0.0f) {
			for (EntityID entity : m_Entities) {
				Update(timeRemaining, entity);//Get components here
			}
		}
	}
}