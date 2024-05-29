//
//	ConstraintMotor.h
//
#pragma once
#include "ConstraintBase.h"

namespace Fengshui
{
	/*
	================================
	ConstraintMotor
	================================
	*/
	class ConstraintMotor : public Constraint {
	public:
		ConstraintMotor() : Constraint(), m_Jacobian( 4, 12 ) {
			m_motorSpeed = 0.0f;
			m_motorAxis = glm::vec3( 0, 0, 1 );
			m_baumgarte = glm::vec3(0.0f);
		}

		void PreSolve( const float dt_sec ) override;
		void Solve() override;

		float m_motorSpeed;
		glm::vec3 m_motorAxis;	// Motor Axis in BodyA's local space
		glm::quat m_q0;		// The initial relative quaternion q1^-1 * q2

		MatMN m_Jacobian;

		glm::vec3 m_baumgarte;
	};
}