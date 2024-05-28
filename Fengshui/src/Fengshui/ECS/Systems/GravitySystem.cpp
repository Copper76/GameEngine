#include "fspch.h"
#include "GravitySystem.h"
#include "Fengshui/Physics/Helper.h"
#include "Fengshui/ECS/GeneralManager.h"

namespace Fengshui
{
	void GravitySystem::OnUpdate(float dt)
	{
		for (EntityID entity : m_Entities) {

			Rigidbody& rb = GeneralManager::GetComponent<Rigidbody>(entity);
			float mass = 1.0f / rb.InvMass;
			const glm::vec3 impulseGravity = rb.Gravity * mass * dt;
			ApplyImpulseLinear(impulseGravity, rb);
			//body->ApplyImpulseLinear(impulseGravity);
		}
	}
}
