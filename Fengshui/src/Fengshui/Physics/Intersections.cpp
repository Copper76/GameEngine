//
//  Intersections.cpp
//
#include "fspch.h"

#include "Intersections.h"
#include "GJK.h"

#include <glm/gtx/norm.hpp>

namespace Fengshui
{
	bool SphereSphereStatic(const PhysicalShapeSphere* sphereA, const PhysicalShapeSphere* sphereB, const glm::vec3& posA, const glm::vec3& posB, glm::vec3& ptOnA, glm::vec3& ptOnB) {
		const glm::vec3 ab = posB - posA;
		glm::vec3 norm = glm::normalize(ab);

		ptOnA = posA + norm * sphereA->m_Radius;
		ptOnB = posB - norm * sphereB->m_Radius;

		const float radiusAB = sphereA->m_Radius + sphereB->m_Radius;
		const float lengthSquare = glm::length2(ab);
		if (lengthSquare <= (radiusAB * radiusAB)) {
			return true;
		}
		return false;
	}

	/*
	====================================================
	Intersect Static
	====================================================
	*/
	bool Intersect(const CollisionPair pair, const Rigidbody bodyA, const Rigidbody bodyB, const Transform transA, const Transform transB, const Collider colliderA, const Collider colliderB, contact_t& contact) 
	{
		contact.bodyA = pair.a;
		contact.bodyB = pair.b;
		contact.timeOfImpact = 0.0f;

		if (colliderA.Shape->GetType() == ShapeType::SHAPE_SPHERE && colliderB.Shape->GetType() == ShapeType::SHAPE_SPHERE) {
			const PhysicalShapeSphere* sphereA = (const PhysicalShapeSphere*)colliderA.Shape;
			const PhysicalShapeSphere* sphereB = (const PhysicalShapeSphere*)colliderB.Shape;

			glm::vec3 posA = transA.Position;
			glm::vec3 posB = transB.Position;

			if (SphereSphereStatic(sphereA, sphereB, posA, posB, contact.ptOnA_WorldSpace, contact.ptOnB_WorldSpace)) {
				contact.normal = glm::normalize(posA - posB);

				contact.ptOnA_LocalSpace = WorldSpaceToBodySpace(contact.ptOnA_WorldSpace, colliderA, transA);
				contact.ptOnB_LocalSpace = WorldSpaceToBodySpace(contact.ptOnB_WorldSpace, colliderB, transB);

				glm::vec3 ab = transB.Position - transA.Position;
				//distance betwen positions minus the radius of both spheres
				float r = glm::length(ab) - (sphereA->m_Radius + sphereB->m_Radius);
				contact.separationDistance = r;
				return true;
			}
		}
		else {
			glm::vec3 ptOnA;
			glm::vec3 ptOnB;
			const float bias = 0.001f;
			if (GJK_DoesIntersect(colliderA, transA, colliderB, transB, bias, ptOnA, ptOnB)) {
				glm::vec3 normal = glm::normalize(ptOnB - ptOnA);

				ptOnA -= normal * bias;
				ptOnB += normal * bias;

				contact.normal = normal;

				contact.ptOnA_WorldSpace = ptOnA;
				contact.ptOnB_WorldSpace = ptOnB;

				contact.ptOnA_LocalSpace = WorldSpaceToBodySpace(contact.ptOnA_WorldSpace, colliderA, transA);
				contact.ptOnB_LocalSpace = WorldSpaceToBodySpace(contact.ptOnB_WorldSpace, colliderB, transB);

				glm::vec3 ab = transB.Position - transA.Position;
				//distance betwen contact points
				float r = glm::length(ptOnA - ptOnB);
				contact.separationDistance = -r;
				return true;
			}
			GJK_ClosestPoints(colliderA, transA, colliderB, transB, ptOnA, ptOnB);
			contact.ptOnA_WorldSpace = ptOnA;
			contact.ptOnB_WorldSpace = ptOnB;

			contact.ptOnA_LocalSpace = WorldSpaceToBodySpace(contact.ptOnA_WorldSpace, colliderA, transA);
			contact.ptOnB_LocalSpace = WorldSpaceToBodySpace(contact.ptOnB_WorldSpace, colliderB, transB);

			glm::vec3 ab = transB.Position - transA.Position;
			//distance betwen contact points
			float r = glm::length(ptOnA - ptOnB);
			contact.separationDistance = r;
		}
		return false;
	}

	bool RaySphere(const glm::vec3& rayStart, const glm::vec3& rayDir, const glm::vec3& sphereCenter, const float sphereRadius, float& t1, float& t2) {
		const glm::vec3 m = sphereCenter - rayStart;
		const float a = glm::dot(rayDir, rayDir);
		const float b = glm::dot(m, rayDir);
		const float c = glm::dot(m, m) - sphereRadius * sphereRadius;

		const float delta = b * b - a * c;
		const float invA = 1.0f / a;

		if (delta < 0) {
			// no real solutions
			return false;
		}

		const float deltaRoot = sqrtf(delta);
		t1 = invA * (b - deltaRoot);
		t2 = invA * (b + deltaRoot);
		return true;
	}

	bool SphereSphereDynamic(const PhysicalShapeSphere* shapeA, const PhysicalShapeSphere* shapeB, const glm::vec3& posA, const glm::vec3& posB, const glm::vec3& velA, const glm::vec3& velB, const float dt, glm::vec3& ptOnA, glm::vec3& ptOnB, float& toi) {
		const glm::vec3 relativeVelocity = velA - velB;

		const glm::vec3 startPtA = posA;
		const glm::vec3 endPtA = posA + relativeVelocity * dt;
		const glm::vec3 rayDir = endPtA - startPtA;

		float t0 = 0;
		float t1 = 0;
		//too close, check collision now
		if (glm::length2(rayDir) < 0.001f * 0.001f) {
			glm::vec3 ab = posB - posA;
			float radius = shapeA->m_Radius + shapeB->m_Radius + 0.001f;
			if (glm::length2(ab) > radius * radius) {
				return false;
			}
		}
		//not going to collide ever
		else if (!RaySphere(posA, rayDir, posB, shapeA->m_Radius + shapeB->m_Radius, t0, t1)) {
			return false;
		}

		t0 *= dt;
		t1 *= dt;

		//no collision in the future
		if (t1 < 0.0f) {
			return false;
		}

		//find the earliest collision (time of impact)
		toi = (t0 < 0.0f) ? 0.0f : t0;

		//collision is too far in the future for us to care
		if (toi > dt) {
			return false;
		}

		//preemptively move the point of contact to when the bodies collide
		glm::vec3 newPosA = posA + velA * toi;
		glm::vec3 newPosB = posB + velB * toi;
		glm::vec3 ab = glm::normalize(newPosB - newPosA);

		ptOnA = newPosA + ab * shapeA->m_Radius;
		ptOnB = newPosB - ab * shapeB->m_Radius;
		return true;
	}



	//dynamic
	bool Intersect(const CollisionPair pair, const float dt, contact_t& contact) {
		Rigidbody& bodyA = GeneralManager::GetComponent<Rigidbody>(contact.bodyA);
		Rigidbody& bodyB = GeneralManager::GetComponent<Rigidbody>(contact.bodyB);

		if (bodyA.InvMass == 0.0f && bodyB.InvMass == 0.0f)
		{
			return false;
		}

		float dt_sec = dt;

		contact.bodyA = pair.a;
		contact.bodyB = pair.b;

		Transform& transA = GeneralManager::GetComponent<Transform>(contact.bodyA);
		Transform& transB = GeneralManager::GetComponent<Transform>(contact.bodyB);

		const Transform& globalTransA = TransformSystem::GetWorldTransform(contact.bodyA);
		const Transform& globalTransB = TransformSystem::GetWorldTransform(contact.bodyB);

		const Collider colliderA = GeneralManager::GetComponent<Collider>(contact.bodyA);
		const Collider colliderB = GeneralManager::GetComponent<Collider>(contact.bodyB);

		if (colliderA.Shape->GetType() == ShapeType::SHAPE_SPHERE && colliderB.Shape->GetType() == ShapeType::SHAPE_SPHERE) {
			const PhysicalShapeSphere* sphereA = (const PhysicalShapeSphere*)colliderA.Shape;
			const PhysicalShapeSphere* sphereB = (const PhysicalShapeSphere*)colliderB.Shape;

			glm::vec3 posA = globalTransA.Position;
			glm::vec3 posB = globalTransB.Position;

			glm::vec3 velA = bodyA.LinearVelocity;
			glm::vec3 velB = bodyB.LinearVelocity;

			if (SphereSphereDynamic(sphereA, sphereB, posA, posB, velA, velB, dt_sec, contact.ptOnA_WorldSpace, contact.ptOnB_WorldSpace, contact.timeOfImpact)) {
				Update(contact.timeOfImpact, colliderA, bodyA, transA, globalTransA.Rotation);
				Update(contact.timeOfImpact, colliderB, bodyB, transB, globalTransB.Rotation);

				contact.ptOnA_LocalSpace = WorldSpaceToBodySpace(contact.ptOnA_WorldSpace, colliderA, transA);
				contact.ptOnB_LocalSpace = WorldSpaceToBodySpace(contact.ptOnA_WorldSpace, colliderB, transB);

				contact.normal = glm::normalize(globalTransA.Position - globalTransB.Position);

				Update(-contact.timeOfImpact, colliderA, bodyA, transA, globalTransA.Rotation);
				Update(-contact.timeOfImpact, colliderB, bodyB, transB, globalTransB.Rotation);

				glm::vec3 ab = globalTransB.Position - globalTransA.Position;
				//distance betwen positions minus the radius of both spheres
				float r = glm::length(ab) - (sphereA->m_Radius + sphereB->m_Radius);
				contact.separationDistance = r;
				return true;
			}
		}
		else {
			float toi = 0.0f;

			int numIters = 0;

			//Advance the positions of the bodies until they touch or there is no time left
			while (dt_sec > 0.0f) {
				bool didIntersect = Intersect(pair,bodyA, bodyB, globalTransA, globalTransB, colliderA, colliderB, contact);
				if (didIntersect) {
					contact.timeOfImpact = toi;
					//move bodies back to where they were
					Update(-toi, colliderA, bodyA, transA, globalTransA.Rotation);
					Update(-toi, colliderB, bodyB, transB, globalTransB.Rotation);
					return true;
				}

				++numIters;
				if (numIters > 10) {
					break;
				}

				//get the vector of between the closest points on A and B
				glm::vec3 ab = glm::normalize(contact.ptOnB_WorldSpace - contact.ptOnA_WorldSpace);

				glm::vec3 relativeVelocity = bodyA.LinearVelocity - bodyB.LinearVelocity;
				float orthoSpeed = glm::dot(relativeVelocity, ab);

				float angularSpeedA = colliderA.Shape->FastestLinearSpeed(bodyA.AngularVelocity, ab);
				float angularSpeedB = colliderB.Shape->FastestLinearSpeed(bodyB.AngularVelocity, ab * -1.0f);
				orthoSpeed += angularSpeedA + angularSpeedB;
				//not going towards each other so no collision
				if (orthoSpeed <= 0.0f) {
					break;
				}

				float timeToGo = contact.separationDistance / orthoSpeed;
				if (timeToGo > dt_sec) {
					break;
				}

				dt_sec -= timeToGo;//wouldn't this move the objecs together in one step?, why the iterations then? Right, it is to avoid calculating objects that are spinning fast
				toi += timeToGo;
				Update(timeToGo, colliderA, bodyA, transA, globalTransA.Rotation);
				Update(timeToGo, colliderB, bodyB, transB, globalTransB.Rotation);
			}

			Update(-toi, colliderA, bodyA, transA, globalTransA.Rotation);
			Update(-toi, colliderB, bodyB, transB, globalTransB.Rotation);
			return false;
		}
		return false;
	}
}