#pragma once
#include "Fengshui/ECS/ECS.h"
#include "Fengshui/Physics/Physics/Constraints.h"
#include "Fengshui/Physics/Physics/Manifold.h"

namespace Fengshui
{
	class CollisionSystem : public System
	{
	public:
		void OnUpdate(float dt);

		void AddConstraint(Constraint* constraint);
		void RemoveConstraint(Constraint* contraint);

	private:
		std::vector< Constraint* > m_Constraints;
		ManifoldCollector m_Manifolds;
	};

}
