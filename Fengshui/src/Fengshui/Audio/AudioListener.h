#pragma once

#include <glm/glm.hpp>

namespace Fengshui
{
	class AudioListener
	{
	public:
		virtual void UpdateLocation(glm::vec3 position) = 0;
		virtual void UpdateOrientation(glm::vec3 forward, glm::vec3 up) = 0;
	};
}