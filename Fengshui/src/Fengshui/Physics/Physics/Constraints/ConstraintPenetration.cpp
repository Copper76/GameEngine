//
//  ConstraintPenetration.cpp
//
#include "fspch.h"

#include "ConstraintPenetration.h"

namespace Fengshui
{
	/*
	================================
	ConstraintPenetration::PreSolve
	================================
	*/
	void ConstraintPenetration::PreSolve(const float dt_sec) 
	{
		const Collider colliderA = GeneralManager::GetComponent<Collider>(m_BodyA);
		const Rigidbody bodyA = GeneralManager::GetComponent<Rigidbody>(m_BodyA);
		const Transform transA = GeneralManager::GetComponent<Transform>(m_BodyA);

		const Collider colliderB = GeneralManager::GetComponent<Collider>(m_BodyB);
		const Rigidbody bodyB = GeneralManager::GetComponent<Rigidbody>(m_BodyB);
		const Transform transB = GeneralManager::GetComponent<Transform>(m_BodyB);

		const glm::vec3 worldAnchorA = BodySpaceToWorldSpace(m_anchorA, colliderA, transA);
		const glm::vec3 worldAnchorB = BodySpaceToWorldSpace(m_anchorB, colliderB, transB);

		const glm::vec3 ra = worldAnchorA - GetCenterOfMassWorldSpace(colliderA, transA);
		const glm::vec3 rb = worldAnchorB - GetCenterOfMassWorldSpace(colliderB, transB);

		const float frictionA = bodyA.Friction;
		const float frictionB = bodyB.Friction;
		m_friction = frictionA * frictionB;

		glm::vec3 u;
		glm::vec3 v;
		GetOrtho(m_normal, u, v);//but u and v are undefined?

		glm::vec3 normal = transA.Rotation * m_normal;//why A?
		u = transA.Rotation * u;
		v = transA.Rotation * v;

		m_Jacobian.Zero();

		//row 1 is the primary distance constraint that holds anchor points together
		glm::vec3 J1 = normal * -1.0f;
		m_Jacobian.rows[0][0] = J1.x;
		m_Jacobian.rows[0][1] = J1.y;
		m_Jacobian.rows[0][2] = J1.z;

		glm::vec3 J2 = glm::cross(ra, normal * -1.0f);
		m_Jacobian.rows[0][3] = J2.x;
		m_Jacobian.rows[0][4] = J2.y;
		m_Jacobian.rows[0][5] = J2.z;

		glm::vec3 J3 = normal * 1.0f;
		m_Jacobian.rows[0][6] = J3.x;
		m_Jacobian.rows[0][7] = J3.y;
		m_Jacobian.rows[0][8] = J3.z;

		glm::vec3 J4 = glm::cross(rb, normal * 1.0f);
		m_Jacobian.rows[0][9] = J4.x;
		m_Jacobian.rows[0][10] = J4.y;
		m_Jacobian.rows[0][11] = J4.z;

		//Friction Jacobian
		if (m_friction > 0.0f) {
			glm::vec3 J1 = u * -1.0f;
			m_Jacobian.rows[1][0] = J1.x;
			m_Jacobian.rows[1][1] = J1.y;
			m_Jacobian.rows[1][2] = J1.z;

			glm::vec3 J2 = glm::cross(ra, u * -1.0f);
			m_Jacobian.rows[1][3] = J2.x;
			m_Jacobian.rows[1][4] = J2.y;
			m_Jacobian.rows[1][5] = J2.z;

			glm::vec3 J3 = u * 1.0f;
			m_Jacobian.rows[1][6] = J3.x;
			m_Jacobian.rows[1][7] = J3.y;
			m_Jacobian.rows[1][8] = J3.z;

			glm::vec3 J4 = glm::cross(rb, u * 1.0f);
			m_Jacobian.rows[1][9] = J4.x;
			m_Jacobian.rows[1][10] = J4.y;
			m_Jacobian.rows[1][11] = J4.z;

			J1 = v * -1.0f;
			m_Jacobian.rows[2][0] = J1.x;
			m_Jacobian.rows[2][1] = J1.y;
			m_Jacobian.rows[2][2] = J1.z;

			J2 = glm::cross(ra, v * -1.0f);
			m_Jacobian.rows[2][3] = J2.x;
			m_Jacobian.rows[2][4] = J2.y;
			m_Jacobian.rows[2][5] = J2.z;

			J3 = v * 1.0f;
			m_Jacobian.rows[2][6] = J3.x;
			m_Jacobian.rows[2][7] = J3.y;
			m_Jacobian.rows[2][8] = J3.z;

			J4 = glm::cross(rb, v * 1.0f);
			m_Jacobian.rows[2][9] = J4.x;
			m_Jacobian.rows[2][10] = J4.y;
			m_Jacobian.rows[2][11] = J4.z;
		}

		//Apply warm start (don't need post solve to resolve chain issues?)
		ApplyImpulses(m_Jacobian.Transpose() * m_cachedLambda);

		//Calculate Baumgarte stabilisation
		float C = glm::dot((worldAnchorB - worldAnchorA), normal);
		C = std::min(0.0f, C + 0.02f);//ensure non-negative, then, if the value is very close to 0, no correction needed
		const float Beta = 0.25f;//manually chosen beta factor so we don't add a lot of energy but still corrects the constraint
		m_baumgarte = Beta * C / dt_sec;
	}

	/*
	================================
	ConstraintPenetration::Solve
	================================
	*/
	void ConstraintPenetration::Solve() {
		const MatMN JacobianTranspose = m_Jacobian.Transpose();

		const VecN q_dt = GetVelocities();
		const MatMN invMassMatrix = GetInverseMassMatrix();
		const MatMN J_W_Jt = m_Jacobian * invMassMatrix * JacobianTranspose;//denominator
		VecN rhs = m_Jacobian * q_dt * -1.0f; //-J*v (numerator)
		rhs[0] -= m_baumgarte;

		//calculate lagrange multiplier
		VecN lambdaN = LCP_GaussSeidel(J_W_Jt, rhs);

		//accumulation, interesting
		VecN oldLambda = m_cachedLambda;
		m_cachedLambda += lambdaN;
		const float lambdaLimit = 0.0f;
		if (m_cachedLambda[0] < lambdaLimit) {
			m_cachedLambda[0] = lambdaLimit;//cap the lambda to be non-negative
		}
		if (m_friction > 0.0f) {
			const Rigidbody bodyA = GeneralManager::GetComponent<Rigidbody>(m_BodyA);
			const Rigidbody bodyB = GeneralManager::GetComponent<Rigidbody>(m_BodyB);

			const float umg = m_friction * 10.0f / (bodyA.InvMass + bodyB.InvMass);//why times by 10
			const float normalForce = fabsf(lambdaN[0] * m_friction);
			const float maxForce = (umg > normalForce) ? umg : normalForce;

			if (m_cachedLambda[1] > maxForce) {
				m_cachedLambda[1] = maxForce;
			}
			if (m_cachedLambda[1] < -maxForce) {
				m_cachedLambda[1] = -maxForce;
			}
			if (m_cachedLambda[2] > maxForce) {
				m_cachedLambda[2] = maxForce;
			}
			if (m_cachedLambda[2] < -maxForce) {
				m_cachedLambda[2] = -maxForce;
			}
		}
		lambdaN = m_cachedLambda - oldLambda;

		//Qint = JT * lambda
		ApplyImpulses(JacobianTranspose * lambdaN);
	}
}