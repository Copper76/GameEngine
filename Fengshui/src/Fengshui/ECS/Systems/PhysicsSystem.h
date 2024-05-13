#pragma once
#include "Fengshui/ECS/ECS.h"
#include "Fengshui/Physics/Physics/Constraints.h"
#include "Fengshui/Physics/Physics/Manifold.h"

namespace Fengshui
{
	class PhysicsSystem : public System
	{
	public:
		void OnUpdate(float dt);

	private:
		void UpdateInternal(float dt_sec);

	private:
		float elapsedTime;
		std::vector< Constraint* >	m_constraints;
		ManifoldCollector m_manifolds;
	};
}
