//
//	ConstraintBase.h
//
#pragma once
#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"
#include "../../Math/Bounds.h"
#include "../../Math/LCP.h"
#include "Fengshui/ECS/ECS.h"
#include "Fengshui/Physics/Helper.h"
#include <vector>

namespace Fengshui
{
	/*
====================================================
Constraint
====================================================
*/
	class Constraint {
	public:
		virtual void PreSolve(const float dt_sec) {}
		virtual void Solve() {}
		virtual void PostSolve() {}

		static glm::mat4 Left(const glm::quat& q);
		static glm::mat4 Right(const glm::quat& q);

	protected:
		MatMN GetInverseMassMatrix() const;
		VecN GetVelocities() const;
		void ApplyImpulses(const VecN& impulses);

	public:
		EntityID m_BodyA;
		EntityID m_BodyB;

		glm::vec3 m_anchorA;		// The anchor location in bodyA's space
		glm::vec3 m_axisA;		// The axis direction in bodyA's space

		glm::vec3 m_anchorB;		// The anchor location in bodyB's space
		glm::vec3 m_axisB;		// The axis direction in bodyB's space
	};

	/*
	====================================================
	Constraint::GetInverseMassMatrix
	====================================================
	*/
	inline MatMN Constraint::GetInverseMassMatrix() const {
		MatMN invMassMatrix(12, 12);
		invMassMatrix.Zero();

		const Collider colliderA = GeneralManager::GetComponent<Collider>(m_BodyA);
		const Rigidbody bodyA = GeneralManager::GetComponent<Rigidbody>(m_BodyA);
		const Transform transA = GeneralManager::GetComponent<Transform>(m_BodyA);

		const Collider colliderB = GeneralManager::GetComponent<Collider>(m_BodyB);
		const Rigidbody bodyB = GeneralManager::GetComponent<Rigidbody>(m_BodyB);
		const Transform transB = GeneralManager::GetComponent<Transform>(m_BodyB);

		invMassMatrix.rows[0][0] = bodyA.InvMass;
		invMassMatrix.rows[1][1] = bodyA.InvMass;
		invMassMatrix.rows[2][2] = bodyA.InvMass;

		glm::mat3 invInertiaA = GetInverseInertiaTensorWorldSpace(colliderA, bodyA, transA);
		for (int i = 0; i < 3; i++) {
			invMassMatrix.rows[i + 3][3] = invInertiaA[i][0];
			invMassMatrix.rows[i + 3][4] = invInertiaA[i][1];
			invMassMatrix.rows[i + 3][5] = invInertiaA[i][2];
		}

		invMassMatrix.rows[6][6] = bodyB.InvMass;
		invMassMatrix.rows[7][7] = bodyB.InvMass;
		invMassMatrix.rows[8][8] = bodyB.InvMass;

		glm::mat3 invInertiaB = GetInverseInertiaTensorWorldSpace(colliderB, bodyB, transB);
		for (int i = 0; i < 3; i++) {
			invMassMatrix.rows[i + 9][9] = invInertiaB[i][0];
			invMassMatrix.rows[i + 9][10] = invInertiaB[i][1];
			invMassMatrix.rows[i + 9][11] = invInertiaB[i][2];
		}

		return invMassMatrix;
	}

	/*
	====================================================
	Constraint::GetVelocities
	====================================================
	*/
	inline VecN Constraint::GetVelocities() const {
		VecN q_dt(12);

		const Rigidbody bodyA = GeneralManager::GetComponent<Rigidbody>(m_BodyA);
		const Rigidbody bodyB = GeneralManager::GetComponent<Rigidbody>(m_BodyB);

		q_dt[0] = bodyA.LinearVelocity.x;
		q_dt[1] = bodyA.LinearVelocity.y;
		q_dt[2] = bodyA.LinearVelocity.z;

		q_dt[3] = bodyA.AngularVelocity.x;
		q_dt[4] = bodyA.AngularVelocity.y;
		q_dt[5] = bodyA.AngularVelocity.z;

		q_dt[6] = bodyB.LinearVelocity.x;
		q_dt[7] = bodyB.LinearVelocity.y;
		q_dt[8] = bodyB.LinearVelocity.z;

		q_dt[9] = bodyB.AngularVelocity.x;
		q_dt[10] = bodyB.AngularVelocity.y;
		q_dt[11] = bodyB.AngularVelocity.z;

		return q_dt;
	}

	/*
	====================================================
	Constraint::ApplyImpulses
	====================================================
	*/
	inline void Constraint::ApplyImpulses(const VecN& impulses) {
		glm::vec3 tempForce(0.0f);

		Rigidbody& bodyA = GeneralManager::GetComponent<Rigidbody>(m_BodyA);
		Rigidbody& bodyB = GeneralManager::GetComponent<Rigidbody>(m_BodyB);

		const Collider colliderA = GeneralManager::GetComponent<Collider>(m_BodyA);
		const Transform transA = GeneralManager::GetComponent<Transform>(m_BodyA);

		const Collider colliderB = GeneralManager::GetComponent<Collider>(m_BodyB);
		const Transform transB = GeneralManager::GetComponent<Transform>(m_BodyB);

		//changed here to reduce amount of memory used
		tempForce[0] = impulses[0];
		tempForce[1] = impulses[1];
		tempForce[2] = impulses[2];

		ApplyImpulseLinear(tempForce, bodyA);

		tempForce[0] = impulses[3];
		tempForce[1] = impulses[4];
		tempForce[2] = impulses[5];

		ApplyImpulseAngular(tempForce, colliderA, bodyA, transA);

		tempForce[0] = impulses[6];
		tempForce[1] = impulses[7];
		tempForce[2] = impulses[8];

		ApplyImpulseLinear(tempForce, bodyB);

		tempForce[0] = impulses[9];
		tempForce[1] = impulses[10];
		tempForce[2] = impulses[11];

		ApplyImpulseAngular(tempForce, colliderB, bodyB, transB);
	}

	/*
	====================================================
	Constraint::Left
	====================================================
	*/
	inline glm::mat4 Constraint::Left(const glm::quat& q) {
		glm::mat4 L;
		L[0] = glm::vec4(q.w, -q.x, -q.y, -q.z);
		L[1] = glm::vec4(q.x, q.w, -q.z, q.y);
		L[2] = glm::vec4(q.y, q.z, q.w, -q.x);
		L[3] = glm::vec4(q.z, -q.y, q.x, q.w);

		return glm::transpose(L);
	}

	/*
	====================================================
	Constraint::Right
	====================================================
	*/
	inline glm::mat4 Constraint::Right(const glm::quat& q) {
		glm::mat4 R;
		R[0] = glm::vec4(q.w, -q.x, -q.y, -q.z);
		R[1] = glm::vec4(q.x, q.w, q.z, -q.y);
		R[2] = glm::vec4(q.y, -q.z, q.w, q.x);
		R[3] = glm::vec4(q.z, q.y, -q.x, q.w);

		return glm::transpose(R);
	}
}