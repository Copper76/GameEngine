//
//  ConstraintOrientation.cpp
//
#include "fspch.h"

#include "ConstraintOrientation.h"

/*
================================
ConstraintOrientation::PreSolve
================================
*/
void ConstraintOrientation::PreSolve( const float dt_sec ) {
	const Vec3 worldAnchorA = m_bodyA->BodySpaceToWorldSpace(m_anchorA);
	const Vec3 worldAnchorB = m_bodyB->BodySpaceToWorldSpace(m_anchorB);

	const Vec3 r = worldAnchorB - worldAnchorA;
	const Vec3 ra = worldAnchorA - m_bodyA->GetCenterOfMassWorldSpace();
	const Vec3 rb = worldAnchorB - m_bodyB->GetCenterOfMassWorldSpace();

	const Quat q1 = m_bodyA->m_orientation;
	const Quat q2 = m_bodyB->m_orientation;
	const Quat q0_inv = m_q0.Inverse();
	const Quat q1_inv = q1.Inverse();

	const Vec3 u = Vec3(1, 0, 0);
	const Vec3 v = Vec3(0, 1, 0);
	const Vec3 w = Vec3(0, 0, 1);

	Mat4 P;
	P.rows[0] = Vec4(0.0f);
	P.rows[1] = Vec4(0, 1, 0, 0);
	P.rows[2] = Vec4(0, 0, 1, 0);
	P.rows[3] = Vec4(0, 0, 0, 1);
	Mat4 P_T = P.Transpose();

	const Mat4 MatA = P * Left(q1_inv) * Right(q2 * q0_inv) * P_T * -0.5f;
	const Mat4 MatB = P * Left(q1_inv) * Right(q2 * q0_inv) * P_T * 0.5f;

	m_Jacobian.Zero();

	//first row is still distance constraint
	Vec3 J1 = (worldAnchorA - worldAnchorB) * 2.0f;
	m_Jacobian.rows[0][0] = J1.x;
	m_Jacobian.rows[0][1] = J1.y;
	m_Jacobian.rows[0][2] = J1.z;

	Vec3 J2 = ra.Cross((worldAnchorA - worldAnchorB) * 2.0f);
	m_Jacobian.rows[0][3] = J2.x;
	m_Jacobian.rows[0][4] = J2.y;
	m_Jacobian.rows[0][5] = J2.z;

	Vec3 J3 = (worldAnchorB - worldAnchorA) * 2.0f;
	m_Jacobian.rows[0][6] = J3.x;
	m_Jacobian.rows[0][7] = J3.y;
	m_Jacobian.rows[0][8] = J3.z;

	Vec3 J4 = rb.Cross((worldAnchorB - worldAnchorA) * 2.0f);
	m_Jacobian.rows[0][9] = J4.x;
	m_Jacobian.rows[0][10] = J4.y;
	m_Jacobian.rows[0][11] = J4.z;

	//the rest are the three imaginary parts of the quaternion, representing the three axis
	const int idx = 1;

	Vec4 tmp;
	{
		J1.Zero();
		m_Jacobian.rows[1][0] = J1.x;
		m_Jacobian.rows[1][1] = J1.y;
		m_Jacobian.rows[1][2] = J1.z;

		tmp = MatA * Vec4(0, u.x, u.y, u.z);
		J2 = Vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
		m_Jacobian.rows[1][3] = J2.x;
		m_Jacobian.rows[1][4] = J2.y;
		m_Jacobian.rows[1][5] = J2.z;

		J3.Zero();
		m_Jacobian.rows[1][6] = J3.x;
		m_Jacobian.rows[1][7] = J3.y;
		m_Jacobian.rows[1][8] = J3.z;

		tmp = MatB * Vec4(0, u.x, u.y, u.z);
		J4 = Vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
		m_Jacobian.rows[1][9] = J4.x;
		m_Jacobian.rows[1][10] = J4.y;
		m_Jacobian.rows[1][11] = J4.z;
	}
	{
		J1.Zero();
		m_Jacobian.rows[2][0] = J1.x;
		m_Jacobian.rows[2][1] = J1.y;
		m_Jacobian.rows[2][2] = J1.z;

		tmp = MatA * Vec4(0, v.x, v.y, v.z);
		J2 = Vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
		m_Jacobian.rows[2][3] = J2.x;
		m_Jacobian.rows[2][4] = J2.y;
		m_Jacobian.rows[2][5] = J2.z;

		J3.Zero();
		m_Jacobian.rows[2][6] = J3.x;
		m_Jacobian.rows[2][7] = J3.y;
		m_Jacobian.rows[2][8] = J3.z;

		tmp = MatB * Vec4(0, v.x, v.y, v.z);
		J4 = Vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
		m_Jacobian.rows[2][9] = J4.x;
		m_Jacobian.rows[2][10] = J4.y;
		m_Jacobian.rows[2][11] = J4.z;
	}
	{
		J1.Zero();
		m_Jacobian.rows[3][0] = J1.x;
		m_Jacobian.rows[3][1] = J1.y;
		m_Jacobian.rows[3][2] = J1.z;

		tmp = MatA * Vec4(0, w.x, w.y, w.z);
		J2 = Vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
		m_Jacobian.rows[3][3] = J2.x;
		m_Jacobian.rows[3][4] = J2.y;
		m_Jacobian.rows[3][5] = J2.z;

		J3.Zero();
		m_Jacobian.rows[3][6] = J3.x;
		m_Jacobian.rows[3][7] = J3.y;
		m_Jacobian.rows[3][8] = J3.z;

		tmp = MatB * Vec4(0, w.x, w.y, w.z);
		J4 = Vec3(tmp[idx + 0], tmp[idx + 1], tmp[idx + 2]);
		m_Jacobian.rows[3][9] = J4.x;
		m_Jacobian.rows[3][10] = J4.y;
		m_Jacobian.rows[3][11] = J4.z;
	}

	float C = r.Dot(r);
	const float Beta = 0.5f;//manually chosen beta factor so we don't add a lot of energy but still corrects the constraint
	m_baumgarte = Beta * C / dt_sec;
}

/*
================================
ConstraintOrientation::Solve
================================
*/
void ConstraintOrientation::Solve() {
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
}