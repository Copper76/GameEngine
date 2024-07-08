//
//  ConstraintHinge.cpp
//
#include "fspch.h"

#include "ConstraintHinge.h"

namespace Fengshui
{
	/*
	================================================================

	ConstraintHingeglm::quat

	================================================================
	*/

	/*
	================================
	ConstraintHingeglm::quat::PreSolve
	================================
	*/
	void ConstraintHingeQuat::PreSolve(const float dt_sec)
	{
		const Collider& colliderA = GeneralManager::GetComponent<Collider>(m_BodyA);
		const Transform& transA = TransformSystem::GetWorldTransform(m_BodyA);

		const Collider& colliderB = GeneralManager::GetComponent<Collider>(m_BodyB);
		const Transform& transB = TransformSystem::GetWorldTransform(m_BodyB);

		const glm::vec3 worldAnchorA = BodySpaceToWorldSpace(m_anchorA, colliderA, transA);
		const glm::vec3 worldAnchorB = BodySpaceToWorldSpace(m_anchorB, colliderB, transB);

		const glm::vec3 r = worldAnchorB - worldAnchorA;
		const glm::vec3 ra = worldAnchorA - GetCenterOfMassWorldSpace(colliderA, transA);
		const glm::vec3 rb = worldAnchorB - GetCenterOfMassWorldSpace(colliderB, transB);

		const glm::quat q1 = transA.Rotation;
		const glm::quat q2 = transB.Rotation;
		const glm::quat q0_inv = glm::inverse(m_q0);
		const glm::quat q1_inv = glm::inverse(q1);

		glm::vec3 u;//u and v directions need to be found, defined in the local space of body A
		glm::vec3 v;
		glm::vec3 hingeAxis = m_axisA;
		GetOrtho(hingeAxis, u, v);

		glm::mat4 P;
		P[0] = glm::vec4(0.0f);
		P[1] = glm::vec4(0, 1, 0, 0);
		P[2] = glm::vec4(0, 0, 1, 0);
		P[3] = glm::vec4(0, 0, 0, 1);
		glm::mat4 P_T = glm::transpose(P);

		const glm::mat4 MatA = P * Left(q1_inv) * Right(q2 * q0_inv) * P_T * -0.5f;
		const glm::mat4 MatB = P * Left(q1_inv) * Right(q2 * q0_inv) * P_T * 0.5f;

		m_Jacobian.Zero();

		//Again distance for row 1
		glm::vec3 J1 = (worldAnchorA - worldAnchorB) * 2.0f;
		m_Jacobian.rows[0][0] = J1.x;
		m_Jacobian.rows[0][1] = J1.y;
		m_Jacobian.rows[0][2] = J1.z;

		glm::vec3 J2 = glm::cross(ra, (worldAnchorA - worldAnchorB) * 2.0f);
		m_Jacobian.rows[0][3] = J2.x;
		m_Jacobian.rows[0][4] = J2.y;
		m_Jacobian.rows[0][5] = J2.z;

		glm::vec3 J3 = (worldAnchorB - worldAnchorA) * 2.0f;
		m_Jacobian.rows[0][6] = J3.x;
		m_Jacobian.rows[0][7] = J3.y;
		m_Jacobian.rows[0][8] = J3.z;

		glm::vec3 J4 = glm::cross(rb, (worldAnchorB - worldAnchorA) * 2.0f);
		m_Jacobian.rows[0][9] = J4.x;
		m_Jacobian.rows[0][10] = J4.y;
		m_Jacobian.rows[0][11] = J4.z;

		//the rest are the three imaginary parts of the quaternion, representing the three axis
		const int idx = 1;

		glm::vec4 tmp;
		{
			J1 = glm::vec3(0.0f);
			m_Jacobian.rows[1][0] = J1.x;
			m_Jacobian.rows[1][1] = J1.y;
			m_Jacobian.rows[1][2] = J1.z;

			tmp = MatA * glm::vec4(0, u.x, u.y, u.z);
			J2 = glm::vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
			m_Jacobian.rows[1][3] = J2.x;
			m_Jacobian.rows[1][4] = J2.y;
			m_Jacobian.rows[1][5] = J2.z;

			J3 = glm::vec3(0.0f);
			m_Jacobian.rows[1][6] = J3.x;
			m_Jacobian.rows[1][7] = J3.y;
			m_Jacobian.rows[1][8] = J3.z;

			tmp = MatB * glm::vec4(0, u.x, u.y, u.z);
			J4 = glm::vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
			m_Jacobian.rows[1][9] = J4.x;
			m_Jacobian.rows[1][10] = J4.y;
			m_Jacobian.rows[1][11] = J4.z;
		}
		{
			J1 = glm::vec3(0.0f);
			m_Jacobian.rows[2][0] = J1.x;
			m_Jacobian.rows[2][1] = J1.y;
			m_Jacobian.rows[2][2] = J1.z;

			tmp = MatA * glm::vec4(0, v.x, v.y, v.z);
			J2 = glm::vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
			m_Jacobian.rows[2][3] = J2.x;
			m_Jacobian.rows[2][4] = J2.y;
			m_Jacobian.rows[2][5] = J2.z;

			J3 = glm::vec3(0.0f);
			m_Jacobian.rows[2][6] = J3.x;
			m_Jacobian.rows[2][7] = J3.y;
			m_Jacobian.rows[2][8] = J3.z;

			tmp = MatB * glm::vec4(0, v.x, v.y, v.z);
			J4 = glm::vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
			m_Jacobian.rows[2][9] = J4.x;
			m_Jacobian.rows[2][10] = J4.y;
			m_Jacobian.rows[2][11] = J4.z;
		}

		//apply warm start
		ApplyImpulses(m_Jacobian.Transpose() * m_cachedLambda);

		float C = glm::dot(r, r);
		C = std::max(0.0f, C - 0.01f);
		const float Beta = 0.05f;//manually chosen beta factor so we don't add a lot of energy but still corrects the constraint
		m_baumgarte = Beta * C / dt_sec;
	}

	/*
	================================
	ConstraintHingeQuat::Solve
	================================
	*/
	void ConstraintHingeQuat::Solve() 
	{
		const MatMN JacobianTranspose = m_Jacobian.Transpose();

		const VecN q_dt = GetVelocities();
		const MatMN invMassMatrix = GetInverseMassMatrix();
		const MatMN J_W_Jt = m_Jacobian * invMassMatrix * JacobianTranspose;//denominator
		VecN rhs = m_Jacobian * q_dt * -1.0f; //-J*v (numerator)
		rhs[0] -= m_baumgarte;

		//calculate lagrange multiplier
		const VecN lambdaN = LCP_GaussSeidel(J_W_Jt, rhs);

		//Qint = JT * lambda
		ApplyImpulses(JacobianTranspose * lambdaN);

		//accumulate lambda
		m_cachedLambda += lambdaN;
	}

	/*
	================================
	ConstraintHingeQuat::PostSolve
	================================
	*/
	void ConstraintHingeQuat::PostSolve()
	{
		for (int i = 0; i < m_cachedLambda.N; i++) {
			if (m_cachedLambda[i] * 0.0f != m_cachedLambda[i] * 0.0f) {
				m_cachedLambda[i] = 0.0f;
				continue;
			}
			const float limit = 20.0f;
			if (m_cachedLambda[i] > limit) {
				m_cachedLambda[i] = limit;
			}
			if (m_cachedLambda[i] < -limit) {
				m_cachedLambda[i] = -limit;
			}
		}
	}

	/*
	================================================================

	ConstraintHingeQuatLimited

	================================================================
	*/

	/*
	================================
	ConstraintHingeQuatLimited::PreSolve
	================================
	*/
	void ConstraintHingeQuatLimited::PreSolve(const float dt_sec)
	{
		const Collider& colliderA = GeneralManager::GetComponent<Collider>(m_BodyA);
		const Transform& transA = TransformSystem::GetWorldTransform(m_BodyA);

		const Collider& colliderB = GeneralManager::GetComponent<Collider>(m_BodyB);
		const Transform& transB = TransformSystem::GetWorldTransform(m_BodyB);

		const glm::vec3 worldAnchorA = BodySpaceToWorldSpace(m_anchorA, colliderA, transA);
		const glm::vec3 worldAnchorB = BodySpaceToWorldSpace(m_anchorB, colliderB, transB);

		const glm::vec3 r = worldAnchorB - worldAnchorA;
		const glm::vec3 ra = worldAnchorA - GetCenterOfMassWorldSpace(colliderA, transA);
		const glm::vec3 rb = worldAnchorB - GetCenterOfMassWorldSpace(colliderB, transB);

		const glm::quat q1 = transA.Rotation;
		const glm::quat q2 = transB.Rotation;
		const glm::quat q0_inv = glm::inverse(m_q0);
		const glm::quat q1_inv = glm::inverse(q1);

		glm::vec3 u;//u and v directions need to be found, defined in the local space of body A
		glm::vec3 v;
		glm::vec3 hingeAxis = m_axisA;
		GetOrtho(hingeAxis, u, v);

		glm::mat4 P;
		P[0] = glm::vec4(0.0f);
		P[1] = glm::vec4(0, 1, 0, 0);
		P[2] = glm::vec4(0, 0, 1, 0);
		P[3] = glm::vec4(0, 0, 0, 1);
		glm::mat4 P_T = glm::transpose(P);

		const glm::mat4 MatA = P * Left(q1_inv) * Right(q2 * q0_inv) * P_T * -0.5f;
		const glm::mat4 MatB = P * Left(q1_inv) * Right(q2 * q0_inv) * P_T * 0.5f;

		const float pi = acosf(-1.0f);
		const glm::quat qr = q1_inv * q2;
		const glm::quat qrr = qr * q0_inv;
		m_relativeAngle = 2.0f * asinf(glm::dot(glm::vec3(qrr.x, qrr.y, qrr.z), hingeAxis)) * 180.0f / pi;//convert radian to angle as well

		m_isAngleViolated = (m_relativeAngle > 45.0f || m_relativeAngle < -45.0f);

		m_Jacobian.Zero();

		//Again distance for row 1
		glm::vec3 J1 = (worldAnchorA - worldAnchorB) * 2.0f;
		m_Jacobian.rows[0][0] = J1.x;
		m_Jacobian.rows[0][1] = J1.y;
		m_Jacobian.rows[0][2] = J1.z;

		glm::vec3 J2 = glm::cross(ra, (worldAnchorA - worldAnchorB) * 2.0f);
		m_Jacobian.rows[0][3] = J2.x;
		m_Jacobian.rows[0][4] = J2.y;
		m_Jacobian.rows[0][5] = J2.z;

		glm::vec3 J3 = (worldAnchorB - worldAnchorA) * 2.0f;
		m_Jacobian.rows[0][6] = J3.x;
		m_Jacobian.rows[0][7] = J3.y;
		m_Jacobian.rows[0][8] = J3.z;

		glm::vec3 J4 = glm::cross(rb, (worldAnchorB - worldAnchorA) * 2.0f);
		m_Jacobian.rows[0][9] = J4.x;
		m_Jacobian.rows[0][10] = J4.y;
		m_Jacobian.rows[0][11] = J4.z;

		//the rest are the three imaginary parts of the quaternion, representing the three axis
		const int idx = 1;

		glm::vec4 tmp;
		{
			J1 = glm::vec3(0.0f);
			m_Jacobian.rows[1][0] = J1.x;
			m_Jacobian.rows[1][1] = J1.y;
			m_Jacobian.rows[1][2] = J1.z;

			tmp = MatA * glm::vec4(0, u.x, u.y, u.z);
			J2 = glm::vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
			m_Jacobian.rows[1][3] = J2.x;
			m_Jacobian.rows[1][4] = J2.y;
			m_Jacobian.rows[1][5] = J2.z;

			J3 = glm::vec3(0.0f);
			m_Jacobian.rows[1][6] = J3.x;
			m_Jacobian.rows[1][7] = J3.y;
			m_Jacobian.rows[1][8] = J3.z;

			tmp = MatB * glm::vec4(0, u.x, u.y, u.z);
			J4 = glm::vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
			m_Jacobian.rows[1][9] = J4.x;
			m_Jacobian.rows[1][10] = J4.y;
			m_Jacobian.rows[1][11] = J4.z;
		}
		{
			J1 = glm::vec3(0.0f);
			m_Jacobian.rows[2][0] = J1.x;
			m_Jacobian.rows[2][1] = J1.y;
			m_Jacobian.rows[2][2] = J1.z;

			tmp = MatA * glm::vec4(0, v.x, v.y, v.z);
			J2 = glm::vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
			m_Jacobian.rows[2][3] = J2.x;
			m_Jacobian.rows[2][4] = J2.y;
			m_Jacobian.rows[2][5] = J2.z;

			J3 = glm::vec3(0.0f);
			m_Jacobian.rows[2][6] = J3.x;
			m_Jacobian.rows[2][7] = J3.y;
			m_Jacobian.rows[2][8] = J3.z;

			tmp = MatB * glm::vec4(0, v.x, v.y, v.z);
			J4 = glm::vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
			m_Jacobian.rows[2][9] = J4.x;
			m_Jacobian.rows[2][10] = J4.y;
			m_Jacobian.rows[2][11] = J4.z;
		}
		if (m_isAngleViolated) {
			J1 = glm::vec3(0.0f);
			m_Jacobian.rows[3][0] = J1.x;
			m_Jacobian.rows[3][1] = J1.y;
			m_Jacobian.rows[3][2] = J1.z;

			tmp = MatA * glm::vec4(0, hingeAxis.x, hingeAxis.y, hingeAxis.z);
			J2 = glm::vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
			m_Jacobian.rows[3][3] = J2.x;
			m_Jacobian.rows[3][4] = J2.y;
			m_Jacobian.rows[3][5] = J2.z;

			J3 = glm::vec3(0.0f);
			m_Jacobian.rows[3][6] = J3.x;
			m_Jacobian.rows[3][7] = J3.y;
			m_Jacobian.rows[3][8] = J3.z;

			tmp = MatB * glm::vec4(0, hingeAxis.x, hingeAxis.y, hingeAxis.z);
			J4 = glm::vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
			m_Jacobian.rows[3][9] = J4.x;
			m_Jacobian.rows[3][10] = J4.y;
			m_Jacobian.rows[3][11] = J4.z;
		}

		//apply warm start
		ApplyImpulses(m_Jacobian.Transpose() * m_cachedLambda);

		float C = glm::dot(r, r);
		C = std::max(0.0f, C - 0.01f);
		const float Beta = 0.05f;//manually chosen beta factor so we don't add a lot of energy but still corrects the constraint
		m_baumgarte = Beta * C / dt_sec;
	}

	/*
	================================
	ConstraintHingeglm::quatLimited::Solve
	================================
	*/
	void ConstraintHingeQuatLimited::Solve() {
		const MatMN JacobianTranspose = m_Jacobian.Transpose();

		const VecN q_dt = GetVelocities();
		const MatMN invMassMatrix = GetInverseMassMatrix();
		const MatMN J_W_Jt = m_Jacobian * invMassMatrix * JacobianTranspose;//denominator
		VecN rhs = m_Jacobian * q_dt * -1.0f; //-J*v (numerator)
		rhs[0] -= m_baumgarte;

		//calculate lagrange multiplier
		VecN lambdaN = LCP_GaussSeidel(J_W_Jt, rhs);

		//make sure the torque is restorative
		if (m_isAngleViolated) {
			if (m_relativeAngle > 0.0f) {
				lambdaN[3] = std::min(0.0f, lambdaN[3]);
			}
			if (m_relativeAngle < 0.0f) {
				lambdaN[3] = std::max(0.0f, lambdaN[3]);
			}
		}

		//Qint = JT * lambda
		ApplyImpulses(JacobianTranspose * lambdaN);

		//accumulate lambda
		m_cachedLambda += lambdaN;
	}

	/*
	================================
	ConstraintHingeglm::quatLimited::PostSolve
	================================
	*/
	void ConstraintHingeQuatLimited::PostSolve() {
		for (int i = 0; i < m_cachedLambda.N; i++) {
			if (i > 0) {
				m_cachedLambda[i] = 0.0f;//don't bother warm start angle components
				continue;
			}
			if (m_cachedLambda[i] * 0.0f != m_cachedLambda[i] * 0.0f) {
				m_cachedLambda[i] = 0.0f;
				continue;
			}
			const float limit = 20.0f;
			if (m_cachedLambda[i] > limit) {
				m_cachedLambda[i] = limit;
			}
			if (m_cachedLambda[i] < -limit) {
				m_cachedLambda[i] = -limit;
			}
		}
	}
}