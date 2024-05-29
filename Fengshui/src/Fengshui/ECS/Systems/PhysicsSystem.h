#pragma once
#include "Fengshui/ECS/ECS.h"
#include "Fengshui/Physics/Constraints.h"
#include "Fengshui/Physics/Manifold.h"
#include "Fengshui/ECS/Systems/TransformSystem.h"

namespace Fengshui
{
	class PhysicsSystem : public System
	{
	public:
		void OnUpdate(float dt, Ref<ManifoldCollector> manifolds, std::vector<Ref<Constraint>> constraints);
	};
}
