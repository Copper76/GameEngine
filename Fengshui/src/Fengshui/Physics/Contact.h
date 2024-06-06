//
//	Contact.h
//
#pragma once
#include "Fengshui/ECS/Systems/TransformSystem.h"

namespace Fengshui
{
	struct contact_t {
		glm::vec3 ptOnA_WorldSpace;
		glm::vec3 ptOnB_WorldSpace;
		glm::vec3 ptOnA_LocalSpace;
		glm::vec3 ptOnB_LocalSpace;

		glm::vec3 normal;	// In World Space coordinates
		float separationDistance;	// positive when non-penetrating, negative when penetrating
		float timeOfImpact;

		EntityID bodyA;
		EntityID bodyB;
	};

	void ResolveContact(contact_t& contact);
}