#pragma once

#include "Fengshui/ECS/ECS.h"
#include "Fengshui/ECS/GeneralManager.h"
#include "Fengshui/Physics/Physics/Shapes.h"

#include <glm/gtx/norm.hpp>

namespace Fengshui
{
	struct CollisionPair {
		EntityID a;
		EntityID b;

		bool operator == (const CollisionPair& rhs) const {
			return (((a == rhs.a) && (b == rhs.b)) || ((a == rhs.b) && (b == rhs.a)));
		}
		bool operator != (const CollisionPair& rhs) const {
			return !(*this == rhs);
		}
	};

	static glm::vec3 GetCenterOfMassWorldSpace(const Collider collider, const Transform trans) {
		const glm::vec3 centerOfMass = collider.Shape->GetCenterOfMass(collider.Offset, collider.Size);
		return (trans.Rotation * centerOfMass) * trans.Scale + trans.Position;
	}

	static glm::vec3 GetCenterOfMassModelSpace(const Collider collider) {
		const glm::vec3 centerOfMass = collider.Shape->GetCenterOfMass(collider.Offset, collider.Size);
		return centerOfMass;
	}

	static glm::vec3 WorldSpaceToBodySpace(const glm::vec3& worldPt, const Collider collider, const Transform trans) {
		glm::quat inverseOrient = glm::inverse(trans.Rotation);
		return inverseOrient * (worldPt - GetCenterOfMassWorldSpace(collider, trans));
	}

	static glm::vec3 BodySpaceToWorldSpace(const glm::vec3& worldPt, const Collider collider, const Transform trans) {
		return GetCenterOfMassWorldSpace(collider, trans) + (trans.Rotation * worldPt) * trans.Scale;
	}
	static glm::mat3 GetInverseInertiaTensorBodySpace(const Collider collider, const Rigidbody rb, const Transform trans) {
		Transform& transform = Transform();
		transform.Position = trans.Position + collider.Offset;
		transform.Rotation = trans.Rotation;
		transform.Scale = trans.Scale * collider.Size;
		glm::mat3 inertiaTensor = collider.Shape->InertiaTensor(transform);
		return glm::inverse(inertiaTensor) * rb.InvMass;
	}

	//used with global vectors
	static glm::mat3 GetInverseInertiaTensorWorldSpace(const Collider collider, const Rigidbody rb, const Transform trans) {
		Transform& transform = Transform();
		transform.Position = trans.Position + collider.Offset;
		transform.Rotation = trans.Rotation;
		transform.Scale = trans.Scale * collider.Size;
		glm::mat3 inertiaTensor = collider.Shape->InertiaTensor(transform);
		glm::mat3 invInertiaTensor = glm::inverse(inertiaTensor) * rb.InvMass;
		glm::mat3 orient = glm::mat3_cast(trans.Rotation);
		//the transposed orient converts a global vector to local space for calculation before translating it back into global space with orient
		invInertiaTensor = orient * invInertiaTensor * glm::transpose(orient);
		return invInertiaTensor;
	}

	static void ApplyImpulseLinear(const glm::vec3& impulse, Rigidbody& rb) {
		if (rb.InvMass == 0.0f) { return; }

		//p=mv
		//dp = m*dv
		//dv = dp/m = impulse/m
		rb.LinearVelocity += impulse * rb.InvMass;
	}

	static void ApplyImpulseAngular(const glm::vec3& impulse, const Collider collider, Rigidbody& rb, const Transform trans) {
		if (rb.InvMass == 0.0f) { return; }

		//change in speed = inertia*impulse
		rb.AngularVelocity += GetInverseInertiaTensorWorldSpace(collider, rb, trans) * impulse;

		//A manual upper limit for angular speed
		const float maxAngularSpeed = 30.0f;
		if (glm::length2(rb.AngularVelocity) > maxAngularSpeed * maxAngularSpeed) {
			rb.AngularVelocity = glm::normalize(rb.AngularVelocity);
			rb.AngularVelocity *= maxAngularSpeed;
		}
	}

	static void ApplyImpulse(const glm::vec3& impulsePoint, const glm::vec3& impulse, const Collider collider, Rigidbody& rb, Transform& trans) {
		if (rb.InvMass == 0.0f) { return; }

		ApplyImpulseLinear(impulse, rb);

		glm::vec3 position = GetCenterOfMassWorldSpace(collider, trans);	// applying impulses must produce torques through the center of mass
		glm::vec3 r = impulsePoint - position;
		glm::vec3 dL = glm::cross(r, impulse);	// this is in world space
		ApplyImpulseAngular(dL, collider, rb, trans);
	}

	static void Update(const float dt_sec, const Collider collider, Rigidbody& rb, Transform& trans) {
		trans.Position += rb.LinearVelocity * dt_sec;

		glm::vec3 positionCM = GetCenterOfMassWorldSpace(collider, trans);
		glm::vec3 cmToPos = trans.Position - positionCM;

		Transform& transform = Transform();
		transform.Position = trans.Position + collider.Offset;
		transform.Rotation = trans.Rotation;
		transform.Scale = trans.Scale * collider.Size;

		//Need to update the rotation torque as it needs to be always perpendicular to center of mass
		//alpha is carried over acceleration from last iteration
		glm::mat3 orientation = glm::mat3_cast(trans.Rotation);
		glm::mat3 inertiaTensor = orientation * collider.Shape->InertiaTensor(transform) * glm::transpose(orientation);
		glm::vec3 alpha = glm::inverse(inertiaTensor) * (glm::cross(rb.AngularVelocity, inertiaTensor * rb.AngularVelocity));
		rb.AngularVelocity += alpha * dt_sec;

		glm::vec3 dAngle = rb.AngularVelocity * dt_sec;
		glm::quat dq = glm::quat(dAngle);
		trans.Rotation = dq * trans.Rotation;

		trans.Position = positionCM + (dq * cmToPos);
	}

	static void Update(const float dt_sec, EntityID entity) {
		const Collider collider = GeneralManager::GetComponent<Collider>(entity);
		Rigidbody& rb = GeneralManager::GetComponent<Rigidbody>(entity);
		Transform& trans = GeneralManager::GetComponent<Transform>(entity);

		Update(dt_sec, collider, rb, trans);
	}

	static glm::mat3 Minor(const glm::mat4 matrix, const int i, const int j) {
		glm::mat3 minor;

		int yy = 0;
		for (int y = 0; y < 4; y++) {
			if (y == j) {
				continue;
			}

			int xx = 0;
			for (int x = 0; x < 4; x++) {
				if (x == i) {
					continue;
				}

				minor[xx][yy] = matrix[x][y];
				xx++;
			}

			yy++;
		}
		return minor;
	}

	static float Cofactor(const glm::mat4 matrix, const int i, const int j) {
		const glm::mat3 minor = Minor(matrix, i, j);
		const float C = float(pow(-1, i + 1 + j + 1)) * glm::determinant(minor);
		return C;
	}

	static void GetOrtho(glm::vec3 baseVec, glm::vec3& u, glm::vec3& v) {
		glm::vec3 n = glm::normalize(baseVec);

		const glm::vec3 w = (n.z * n.z > 0.9f * 0.9f) ? glm::vec3(1, 0, 0) : glm::vec3(0, 0, 1);
		u = glm::normalize(glm::cross(w, n));

		v = glm::normalize(glm::cross(n, u));
		u = glm::normalize(glm::cross(v, n));
	}

	static bool IsValid(const glm::vec3 v)
	{
		return !(glm::any(glm::isinf(v)) || glm::any(glm::isnan(v)));
	}
}