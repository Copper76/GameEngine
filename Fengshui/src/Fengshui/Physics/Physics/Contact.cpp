//
//  Contact.cpp
//
#include "fspch.h"

#include "Contact.h"
#include "Fengshui/Physics/Helper.h"

/*
====================================================
ResolveContact
====================================================
*/
//void ResolveContact( contact_t & contact ) {
//	Body* bodyA = contact.bodyA;
//	Body* bodyB = contact.bodyB;
//
//	const Vec3 ptOnA = bodyA->BodySpaceToWorldSpace(contact.ptOnA_LocalSpace);
//	const Vec3 ptOnB = bodyB->BodySpaceToWorldSpace(contact.ptOnB_LocalSpace);
//
//	//primitive elasticity calculation
//	const float elasticityA = bodyA->m_elasticity;
//	const float elasticityB = bodyB->m_elasticity;
//	const float elasticity = elasticityA * elasticityB;
//
//	const float invMassA = bodyA->m_invMass;
//	const float invMassB = bodyB->m_invMass;
//
//	const Mat3 invWorldInertiaA = bodyA->GetInverseInertiaTensorWorldSpace();
//	const Mat3 invWorldInertiaB = bodyB->GetInverseInertiaTensorWorldSpace();
//
//	//calculate the impulse after collision
//	const Vec3 n = contact.normal;
//
//	const Vec3 ra = ptOnA - bodyA->GetCenterOfMassWorldSpace();
//	const Vec3 rb = ptOnB - bodyB->GetCenterOfMassWorldSpace();
//
//	const Vec3 angularJA = (invWorldInertiaA * ra.Cross(n)).Cross(ra);
//	const Vec3 angularJB = (invWorldInertiaB * rb.Cross(n)).Cross(rb);
//	const float angularFactor = (angularJA + angularJB).Dot(n);
//
//	const Vec3 velA = bodyA->m_linearVelocity + bodyA->m_angularVelocity.Cross(ra);
//	const Vec3 velB = bodyB->m_linearVelocity + bodyB->m_angularVelocity.Cross(rb);
//
//	const Vec3 vab = velA - velB;
//	const float ImpulseJ = (1.0f + elasticity) * vab.Dot(n) / (invMassA + invMassB + angularFactor);
//	const Vec3 vectorImpulseJ = n * ImpulseJ;
//
//	bodyA->ApplyImpulse(ptOnA, vectorImpulseJ * -1.0f);
//	bodyB->ApplyImpulse(ptOnB, vectorImpulseJ * 1.0f);
//
//	//calculate impulse caused by friction
//	const float frictionA = bodyA->m_friction;
//	const float frictionB = bodyB->m_friction;
//	const float friction = frictionA * frictionB;
//
//	const Vec3 velNorm = n * n.Dot(vab);
//	const Vec3 velTang = vab - velNorm;
//
//	Vec3 relativeVelTang = velTang;
//	relativeVelTang.Normalize();
//
//	const Vec3 inertiaA = (invWorldInertiaA * ra.Cross(relativeVelTang)).Cross(ra);
//	const Vec3 inertiaB = (invWorldInertiaB * rb.Cross(relativeVelTang)).Cross(rb);
//	const float invInertia = (inertiaA + inertiaB).Dot(relativeVelTang);
//
//	const float reducedMass = 1.0f / (bodyA->m_invMass + bodyB->m_invMass + invInertia);
//	const Vec3 impulseFriction = velTang * reducedMass * friction;
//
//	bodyA->ApplyImpulse(ptOnA, impulseFriction * -1.0f);
//	bodyB->ApplyImpulse(ptOnB, impulseFriction * 1.0f);
//
//	if (0.0f == contact.timeOfImpact) {
//		//move just outside the collision
//		const Vec3 ds = ptOnB - ptOnA;
//
//		const float tA = invMassA / (invMassA + invMassB);
//		const float tB = invMassB / (invMassA + invMassB);
//
//		bodyA->m_position += ds * tA;
//		bodyB->m_position -= ds * tB;
//	}
//}

namespace Fengshui
{
	void ResolveContact(contact_t& contact) {
		Rigidbody& bodyA = GeneralManager::GetComponent<Rigidbody>(contact.bodyA);
		Rigidbody& bodyB = GeneralManager::GetComponent<Rigidbody>(contact.bodyB);

		Transform& transA = GeneralManager::GetComponent<Transform>(contact.bodyA);
		Transform& transB = GeneralManager::GetComponent<Transform>(contact.bodyB);

		const Collider colliderA = GeneralManager::GetComponent<Collider>(contact.bodyA);
		const Collider colliderB = GeneralManager::GetComponent<Collider>(contact.bodyB);

		const glm::vec3 ptOnA = BodySpaceToWorldSpace(contact.ptOnA_LocalSpace, colliderA, transA);
		const glm::vec3 ptOnB = BodySpaceToWorldSpace(contact.ptOnB_LocalSpace, colliderB, transB);

		//primitive elasticity calculation
		const float elasticityA = bodyA.Elasticity;
		const float elasticityB = bodyB.Elasticity;
		const float elasticity = elasticityA * elasticityB;

		const float invMassA = bodyA.InvMass;
		const float invMassB = bodyB.InvMass;

		const glm::mat3 invWorldInertiaA = GetInverseInertiaTensorWorldSpace(colliderA, bodyA, transA);
		const glm::mat3 invWorldInertiaB = GetInverseInertiaTensorWorldSpace(colliderB, bodyB, transB);

		//calculate the impulse after collision
		const glm::vec3 n = contact.normal;

		const glm::vec3 ra = ptOnA - GetCenterOfMassWorldSpace(colliderA, transA);
		const glm::vec3 rb = ptOnB - GetCenterOfMassWorldSpace(colliderB, transB);

		const glm::vec3 angularJA = glm::cross(invWorldInertiaA * glm::cross(ra,n), ra);
		const glm::vec3 angularJB = glm::cross(invWorldInertiaB * glm::cross(rb,n), rb);
		const float angularFactor = glm::dot((angularJA + angularJB),n);

		const glm::vec3 velA = bodyA.LinearVelocity + glm::cross(bodyA.AngularVelocity, ra);
		const glm::vec3 velB = bodyB.LinearVelocity + glm::cross(bodyB.AngularVelocity, rb);

		const glm::vec3 vab = velA - velB;
		const float ImpulseJ = (1.0f + elasticity) * glm::dot(vab, n) / (invMassA + invMassB + angularFactor);
		const glm::vec3 vectorImpulseJ = n * ImpulseJ;

		ApplyImpulse(ptOnA, vectorImpulseJ * -1.0f, colliderA, bodyA, transA);
		ApplyImpulse(ptOnB, vectorImpulseJ * 1.0f, colliderB, bodyB, transB);

		//calculate impulse caused by friction
		const float frictionA = bodyA.Friction;
		const float frictionB = bodyB.Friction;
		const float friction = frictionA * frictionB;

		const glm::vec3 velNorm = n * glm::dot(n, vab);
		const glm::vec3 velTang = vab - velNorm;

		glm::vec3 relativeVelTang = glm::normalize(velTang);

		const glm::vec3 inertiaA = glm::cross(invWorldInertiaA * glm::cross(ra, relativeVelTang), ra);
		const glm::vec3 inertiaB = glm::cross(invWorldInertiaB * glm::cross(rb, relativeVelTang), rb);
		const float invInertia = glm::dot(inertiaA + inertiaB, relativeVelTang);

		const float reducedMass = 1.0f / (bodyA.InvMass + bodyB.InvMass + invInertia);
		const glm::vec3 impulseFriction = velTang * reducedMass * friction;

		ApplyImpulse(ptOnA, impulseFriction * -1.0f, colliderA, bodyA, transA);
		ApplyImpulse(ptOnB, impulseFriction * 1.0f, colliderB, bodyB, transB);

		if (0.0f == contact.timeOfImpact) {
			//move just outside the collision
			const glm::vec3 ds = ptOnB - ptOnA;

			const float tA = invMassA / (invMassA + invMassB);
			const float tB = invMassB / (invMassA + invMassB);

			transA.Position += ds * tA;
			transB.Position -= ds * tB;
		}
	}
}