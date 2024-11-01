#include "fspch.h"

#include "Manifold.h"


/*
================================================================================================

ManifoldCollector

================================================================================================
*/

/*
================================
ManifoldCollector::AddContact
================================
*/
namespace Fengshui
{
	void ManifoldCollector::AddContact(const contact_t& contact) {
		int foundIdx = -1;
		for (int i = 0; i < m_manifolds.size(); i++) {
			if ((m_manifolds[i].m_BodyA == contact.bodyA && m_manifolds[i].m_BodyB == contact.bodyB) || (m_manifolds[i].m_BodyB == contact.bodyA && m_manifolds[i].m_BodyA == contact.bodyB)) {
				foundIdx = i;
				break;
			}
		}

		if (foundIdx >= 0) {
			m_manifolds[foundIdx].AddContact(contact);
		}
		else {
			Manifold manifold;
			manifold.m_BodyA = contact.bodyA;
			manifold.m_BodyB = contact.bodyB;

			manifold.AddContact(contact);
			m_manifolds.push_back(manifold);
		}
	}

	/*
	================================
	ManifoldCollector::RemoveExpired
	================================
	*/
	void ManifoldCollector::RemoveExpired() {
		for (int i = (int)m_manifolds.size() - 1; i >= 0; i--) {
			m_manifolds[i].RemoveExpiredContacts();

			if (m_manifolds[i].m_numContacts == 0) {
				m_manifolds.erase(m_manifolds.begin() + i);
			}
		}
	}

	/*
	================================
	ManifoldCollector::PreSolve
	================================
	*/
	void ManifoldCollector::PreSolve(const float dt_sec) {
		for (int i = 0; i < m_manifolds.size(); i++) {
			m_manifolds[i].PreSolve(dt_sec);
		}
	}

	/*
	================================
	ManifoldCollector::Solve
	================================
	*/
	void ManifoldCollector::Solve() {
		for (int i = 0; i < m_manifolds.size(); i++) {
			m_manifolds[i].Solve();
		}
	}

	/*
	================================
	Manifold::PostSolve
	================================
	*/
	void ManifoldCollector::PostSolve() {
		for (int i = 0; i < m_manifolds.size(); i++) {
			m_manifolds[i].PostSolve();
		}
	}

	/*
	================================================================================================

	Manifold

	================================================================================================
	*/

	/*
	================================
	Manifold::RemoveExpiredContacts
	================================
	*/
	void Manifold::RemoveExpiredContacts() {
		for (int i = 0; i < m_numContacts; i++) {
			contact_t& contact = m_contacts[i];

			const Collider& colliderA = GeneralManager::GetComponent<Collider>(contact.bodyA);
			const Transform& transA = TransformSystem::GetWorldTransform(contact.bodyA);

			const Collider& colliderB = GeneralManager::GetComponent<Collider>(contact.bodyB);
			const Transform& transB = TransformSystem::GetWorldTransform(contact.bodyB);

			const glm::vec3 a = BodySpaceToWorldSpace(contact.ptOnA_LocalSpace, colliderA, transA);
			const glm::vec3 b = BodySpaceToWorldSpace(contact.ptOnB_LocalSpace, colliderB, transB);

			glm::vec3 normal = m_constraints[i].m_normal;//always length 1
			normal = transA.Rotation * normal;

			const glm::vec3 ab = b - a;
			float penetrationDepth = glm::dot(normal, ab);
			glm::vec3 abNormal = normal * penetrationDepth;
			glm::vec3 abTangent = ab - abNormal;

			const float distanceThreshold = 0.02f;//a slack on the distance. We can keep contacts that are close enough.
			if (glm::length2(abTangent) < distanceThreshold * distanceThreshold && penetrationDepth <= 0) {//distance is not very far and not penetrating
				continue;
			}

			//book definition

			for (int j = i; j < MAX_CONTACTS - 1; j++) {
				m_constraints[j] = m_constraints[j + 1];
				m_contacts[j] = m_contacts[j + 1];
			}

			m_numContacts--;
			i--;
		}

		for (int j = m_numContacts; j < MAX_CONTACTS - 1; j++) {
			m_constraints[j].m_cachedLambda.Zero();//zero out lambdas of unused constraints, done only once
		}

	}

	/*
	================================
	Manifold::AddContact
	================================
	*/
	void Manifold::AddContact(const contact_t& contact_old) {
		contact_t contact = contact_old;

		//fix the contact so bodies align with manifold bodies
		if (contact_old.bodyA != m_BodyA || contact_old.bodyB != m_BodyB) //only one is necessary as long as other codes are correct
		{
			contact.ptOnA_LocalSpace = contact_old.ptOnB_LocalSpace;
			contact.ptOnB_LocalSpace = contact_old.ptOnA_LocalSpace;
			contact.ptOnA_WorldSpace = contact_old.ptOnB_WorldSpace;
			contact.ptOnB_WorldSpace = contact_old.ptOnA_WorldSpace;

			contact.bodyA = m_BodyA;
			contact.bodyB = m_BodyB;
		}

		for (int i = 0; i < m_numContacts; i++)
		{
			const Collider& oldColliderA = GeneralManager::GetComponent<Collider>(m_contacts[i].bodyA);
			const Transform& oldTransA = TransformSystem::GetWorldTransform(m_contacts[i].bodyA);

			const Collider& oldColliderB = GeneralManager::GetComponent<Collider>(m_contacts[i].bodyB);
			const Transform& oldTransB = TransformSystem::GetWorldTransform(m_contacts[i].bodyB);

			const Collider& colliderA = GeneralManager::GetComponent<Collider>(contact.bodyA);
			const Transform& transA = TransformSystem::GetWorldTransform(contact.bodyA);

			const Collider& colliderB = GeneralManager::GetComponent<Collider>(contact.bodyB);
			const Transform& transB = TransformSystem::GetWorldTransform(contact.bodyB);

			const glm::vec3 oldA = BodySpaceToWorldSpace(m_contacts[i].ptOnA_LocalSpace, oldColliderA, oldTransA);
			const glm::vec3 oldB = BodySpaceToWorldSpace(m_contacts[i].ptOnB_LocalSpace, oldColliderB, oldTransB);

			const glm::vec3 newA = BodySpaceToWorldSpace(contact.ptOnA_LocalSpace, colliderA, transA);
			const glm::vec3 newB = BodySpaceToWorldSpace(contact.ptOnB_LocalSpace, colliderB, transB);

			const float distanceThreshold = 0.02f;
			if ((glm::length2(newA - oldA) < distanceThreshold * distanceThreshold) || (glm::length2(newB - oldB) < distanceThreshold * distanceThreshold)) {
				return;
			}
		}

		//check what to discard when we are full on contacts
		int newSlot = m_numContacts;

		//book definition
		if (newSlot >= MAX_CONTACTS) {
			glm::vec3 avg = glm::vec3(0.0f);
			for (int i = 0; i < MAX_CONTACTS; i++) {
				avg += m_contacts[i].ptOnA_LocalSpace;
			}
			avg += contact.ptOnA_LocalSpace;
			avg *= 1.0f / (1 + MAX_CONTACTS);

			float minDist = glm::length2(avg - contact.ptOnA_LocalSpace);
			int removeIdx = -1;

			for (int i = 0; i < MAX_CONTACTS; i++) {
				float dist2 = glm::length2(avg - m_contacts[i].ptOnA_LocalSpace);

				if (dist2 < minDist) {
					minDist = dist2;
					removeIdx = i;
				}
			}

			if (removeIdx != -1) {
				newSlot = removeIdx;
			}
			else {
				return;//don't need to add the new point
			}
		}

		m_contacts[newSlot] = contact;

		m_constraints[newSlot].m_BodyA = contact.bodyA;
		m_constraints[newSlot].m_BodyB = contact.bodyB;
		m_constraints[newSlot].m_anchorA = contact.ptOnA_LocalSpace;
		m_constraints[newSlot].m_anchorB = contact.ptOnB_LocalSpace;

		const Transform transA = TransformSystem::GetWorldTransform(m_BodyA);

		//get the normal for constraint
		m_constraints[newSlot].m_normal = glm::normalize(glm::inverse(glm::normalize(transA.Rotation)) * (contact.normal * -1.0f));

		m_constraints[newSlot].m_cachedLambda.Zero();//replaced constraint so no cached lambda here

		if (newSlot == m_numContacts) {//it was a new contact, not a replacement
			m_numContacts++;
		}
	}

	/*
	================================
	Manifold::PreSolve
	================================
	*/
	void Manifold::PreSolve(const float dt_sec) {
		for (int i = 0; i < m_numContacts; i++) {
			m_constraints[i].PreSolve(dt_sec);
		}
	}

	/*
	================================
	Manifold::Solve
	================================
	*/
	void Manifold::Solve() {
		for (int i = 0; i < m_numContacts; i++) {
			m_constraints[i].Solve();
		}
	}

	/*
	================================
	Manifold::PostSolve
	================================
	*/
	void Manifold::PostSolve() {
		for (int i = 0; i < m_numContacts; i++) {
			m_constraints[i].PostSolve();
		}
	}
}