//
//  ConstraintMotor.cpp
//
#include "fspch.h"

#include "ConstraintMotor.h"

namespace Fengshui
{
	/*
	================================
	ConstraintMotor::PreSolve
	================================
	*/
	void ConstraintMotor::PreSolve(const float dt_sec) 
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

		const glm::quat q1 = glm::normalize(transA.Rotation);
		const glm::quat q2 = glm::normalize(transB.Rotation);
		const glm::quat q0_inv = glm::inverse(glm::normalize(m_q0));
		const glm::quat q1_inv = glm::inverse(q1);

		const glm::vec3 motorAxis = transA.Rotation * m_motorAxis;
		glm::vec3 motorU;//u and v directions need to be found, defined in the local space of body A
		glm::vec3 motorV;
		GetOrtho(motorAxis, motorU, motorV);

		glm::vec3 u = motorU;
		glm::vec3 v = motorV;
		glm::vec3 w = motorAxis;

		glm::mat4 P_T;
		P_T[0] = glm::vec4(0.0f);
		P_T[1] = glm::vec4(0, 1, 0, 0);
		P_T[2] = glm::vec4(0, 0, 1, 0);
		P_T[3] = glm::vec4(0, 0, 0, 1);
		glm::mat4 P = glm::transpose(P_T);

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
		{
			J1 = glm::vec3(0.0f);
			m_Jacobian.rows[3][0] = J1.x;
			m_Jacobian.rows[3][1] = J1.y;
			m_Jacobian.rows[3][2] = J1.z;

			tmp = MatA * glm::vec4(0, w.x, w.y, w.z);
			J2 = glm::vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
			m_Jacobian.rows[3][3] = J2.x;
			m_Jacobian.rows[3][4] = J2.y;
			m_Jacobian.rows[3][5] = J2.z;

			J3 = glm::vec3(0.0f);
			m_Jacobian.rows[3][6] = J3.x;
			m_Jacobian.rows[3][7] = J3.y;
			m_Jacobian.rows[3][8] = J3.z;

			tmp = MatB * glm::vec4(0, w.x, w.y, w.z);
			J4 = glm::vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
			m_Jacobian.rows[3][9] = J4.x;
			m_Jacobian.rows[3][10] = J4.y;
			m_Jacobian.rows[3][11] = J4.z;
		}

		float C = glm::dot(r, r);
		const float Beta = 0.05f;//manually chosen beta factor so we don't add a lot of energy but still corrects the constraint

		const glm::quat qr = glm::normalize(glm::inverse(transA.Rotation) * transB.Rotation);
		const glm::quat qrA = glm::normalize(qr * q0_inv);//relative orientation in body A space

		const glm::vec3 axisA = transA.Rotation * glm::vec3(qrA.x, qrA.y, qrA.z);

		m_baumgarte = glm::vec3(0.0f);
		m_baumgarte[0] = Beta * C / dt_sec;
		m_baumgarte[1] = glm::dot(motorU, axisA) * (Beta / dt_sec);
		m_baumgarte[2] = glm::dot(motorV, axisA) * (Beta / dt_sec);
	}

	/*
	================================
	ConstraintMotor::Solve
	================================
	*/
	void ConstraintMotor::Solve() {
		const Transform transA = GeneralManager::GetComponent<Transform>(m_BodyA);

		const glm::vec3 motorAxis = transA.Rotation * m_motorAxis;

		VecN w_dt(12);
		w_dt.Zero();
		w_dt[3] = motorAxis[0] * -m_motorSpeed;
		w_dt[4] = motorAxis[1] * -m_motorSpeed;
		w_dt[5] = motorAxis[2] * -m_motorSpeed;

		w_dt[9] = motorAxis[0] * m_motorSpeed;
		w_dt[10] = motorAxis[1] * m_motorSpeed;
		w_dt[11] = motorAxis[2] * m_motorSpeed;

		const MatMN JacobianTranspose = m_Jacobian.Transpose();

		const VecN q_dt = GetVelocities() - w_dt;
		const MatMN invMassMatrix = GetInverseMassMatrix();
		const MatMN J_W_Jt = m_Jacobian * invMassMatrix * JacobianTranspose;//denominator
		VecN rhs = m_Jacobian * q_dt * -1.0f; //-J*v (numerator)
		for (int i = 0; i < 3; i++) {
			rhs[i] -= m_baumgarte[i];
		}

		//calculate lagrange multiplier
		VecN lambdaN = LCP_GaussSeidel(J_W_Jt, rhs);

		//Qint = JT * lambda
		ApplyImpulses(JacobianTranspose * lambdaN);
	}
}