//
//  ConstraintDistance.cpp
//
#include "fspch.h"

#include "ConstraintDistance.h"

namespace Fengshui
{
	void ConstraintDistance::PreSolve(const float dt_sec)
	{
		const Collider colliderA = GeneralManager::GetComponent<Collider>(m_BodyA);
		const Transform transA = TransformSystem::GetWorldTransform(m_BodyA);

		const Collider colliderB = GeneralManager::GetComponent<Collider>(m_BodyB);
		const Transform transB = TransformSystem::GetWorldTransform(m_BodyB);

		const glm::vec3 worldAnchorA = BodySpaceToWorldSpace(m_anchorA, colliderA, transA);
		const glm::vec3 worldAnchorB = BodySpaceToWorldSpace(m_anchorB, colliderB, transB);

		const glm::vec3 r = worldAnchorB - worldAnchorA;
		const glm::vec3 ra = worldAnchorA - GetCenterOfMassWorldSpace(colliderA, transA);
		const glm::vec3 rb = worldAnchorB - GetCenterOfMassWorldSpace(colliderB, transB);

		m_Jacobian.Zero();

		//a = anchorA, b = anchorB
		//J = (2*(a-b),2*ra x (a-b),2*(b-a),2*rb x (b-a))
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

		//apply lambda from previous frame for a warm start
		ApplyImpulses(m_Jacobian.Transpose() * m_cachedLambda);

		//Calculate Baumgarte stabilisation
		float C = glm::dot(r, r);
		C = std::max(0.0f, C - 0.01f);//ensure non-negative, then, if the value is very close to 0, no correction needed
		const float Beta = 0.05f;//manually chosen beta factor so we don't add a lot of energy but still corrects the constraint
		m_baumgarte = Beta * C / dt_sec;
	}

	void ConstraintDistance::Solve()
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

		//accumulation, interesting
		m_cachedLambda += lambdaN;
	}

	void ConstraintDistance::PostSolve()
	{
		if (m_cachedLambda[0] * 0.0f != m_cachedLambda[0] * 0.0f) {
			m_cachedLambda[0] = 0.0f;
			return;
		}
		const float limit = 1e2f;
		if (m_cachedLambda[0] > limit) {
			m_cachedLambda[0] = limit;
		}
		if (m_cachedLambda[0] < -limit) {
			m_cachedLambda[0] = -limit;
		}
	}
}