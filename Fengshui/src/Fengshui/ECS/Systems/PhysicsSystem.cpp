#include "fspch.h"
#include "PhysicsSystem.h"

#include "Fengshui/Physics/Physics/Contact.h"
#include "Fengshui/Physics/Physics/Intersections.h"
#include "Fengshui/Physics/Physics/Broadphase.h"
#include "Fengshui/Physics/Physics/Shapes.h"

namespace Fengshui
{
	void PhysicsSystem::OnUpdate(float dt)
	{
		float dt_us = dt * 1000000.0f;
		if (dt_us < 16000.0f)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(16000 - (int)dt_us));
			dt_us = 16000;
		}

		//Avoid the time step to be too big
		if (dt_us > 33000.0f)
		{
			dt_us = 33000.0f;
		}

		float dt_sec = dt_us * 0.001f * 0.001f * 0.5f;

		for (int i = 0; i < 2; i++) {
			UpdateInternal(dt_sec * 0.5f);
		}
	}

	void PhysicsSystem::UpdateInternal(float dt_sec)
	{
		/*
		m_manifolds.RemoveExpired();

		//gravity
		for (int i = 0; i < m_bodies.size(); i++) {
			Body* body = &m_bodies[i];
			float mass = 1.0f / body->m_invMass;
			Vec3 impulseGravity = Vec3(0, 0, -10) * mass * dt_sec;
			body->ApplyImpulseLinear(impulseGravity);
		}

		//Broadphase to retain only the possible collsions to reduce collision calculation in the narrow phase
		std::vector< collisionPair_t > collisionPairs;
		BroadPhase(m_bodies.data(), (int)m_bodies.size(), collisionPairs, dt_sec);

		//narrow phase, where general contact and collision are calculated
		int numContacts = 0;
		const int maxContacts = (int)m_bodies.size() * (int)m_bodies.size();
		contact_t* contacts = (contact_t*)alloca(sizeof(contact_t) * maxContacts);
		for (int i = 0; i < collisionPairs.size(); i++) {
			const collisionPair_t& pair = collisionPairs[i];
			Body* bodyA = &m_bodies[pair.a];
			Body* bodyB = &m_bodies[pair.b];

			//skip infinite mass pairs
			if (0.0f == bodyA->m_invMass && 0.0f == bodyB->m_invMass) {
				continue;
			}

			contact_t contact;
			if (Intersect(bodyA, bodyB, dt_sec, contact)) {
				if (contact.timeOfImpact == 0.0f) {
					m_manifolds.AddContact(contact);
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
		}

		//
		//solve constraints for each constraint
		//
		for (int i = 0; i < m_constraints.size(); i++) {
			m_constraints[i]->PreSolve(dt_sec);
		}
		m_manifolds.PreSolve(dt_sec);

		const int numIter = 5;
		for (int j = 0; j < numIter; j++) {
			for (int i = 0; i < m_constraints.size(); i++) {
				m_constraints[i]->Solve();
			}
			m_manifolds.Solve();
		}

		for (int i = 0; i < m_constraints.size(); i++) {
			m_constraints[i]->PostSolve();
		}
		m_manifolds.PostSolve();

		float accumulatedTime = 0.0f;
		for (int i = 0; i < numContacts; i++) {
			contact_t& contact = contacts[i];
			const float dt = contact.timeOfImpact - accumulatedTime;

			//Update the position up to next collision
			for (int j = 0; j < m_bodies.size(); j++) {
				m_bodies[j].Update(dt);
			}
			ResolveContact(contact);
			accumulatedTime += dt;
		}

		//Update the position for the rest of the frame's time
		const float timeRemaining = dt_sec - accumulatedTime;
		if (timeRemaining > 0.0f) {
			for (int i = 0; i < m_bodies.size(); i++) {
				m_bodies[i].Update(timeRemaining);
			}
		}
		*/
	}

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
}