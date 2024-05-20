#include "fspch.h"
#include "GravitySystem.h"
#include "Fengshui/Physics/Helper.h"

namespace Fengshui
{
	void GravitySystem::OnUpdate(float dt)
	{
		for (EntityID entity : m_Entities) {

			Rigidbody& rb = GeneralManager::GetComponent<Rigidbody>(entity);
			float mass = 1.0f / rb.InvMass;
			const glm::vec3 impulseGravity = glm::vec3(0.0f, -rb.Gravity, 0.0f ) * mass * dt;
			ApplyImpulseLinear(impulseGravity, rb);
			//body->ApplyImpulseLinear(impulseGravity);
		}
	}
}
