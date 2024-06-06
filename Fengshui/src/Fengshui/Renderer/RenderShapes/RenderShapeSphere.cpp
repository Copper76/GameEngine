#include "fspch.h"
#include "RenderShapeSphere.h"

#include "Fengshui/Maths/Geometry.h"
#include "glm/gtc/quaternion.hpp"

namespace Fengshui
{
	RenderShapeSphere::RenderShapeSphere(float radius, int divisions) : m_Radius(radius), m_Divisions(divisions)
	{
		Build(radius, divisions);
	}

	void RenderShapeSphere::Build(float radius, int divisions)
	{
		glm::quat* cubeRotation = new glm::quat[6];

		cubeRotation[0] = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

		cubeRotation[1] = glm::quat(glm::radians(glm::vec3(180.0f, 0.0f, 0.0f)));
		cubeRotation[2] = glm::quat(glm::radians(glm::vec3(90.0f, 0.0f, 0.0f)));
		cubeRotation[3] = glm::quat(glm::radians(glm::vec3(-90.0f, 0.0f, 0.0f)));
		cubeRotation[4] = glm::quat(glm::radians(glm::vec3(0.0f, 90.0f, 0.0f)));
		cubeRotation[5] = glm::quat(glm::radians(glm::vec3(0.0f, -90.0f, 0.0f)));

		const int numVertsOneFace = (divisions + 1) * (divisions + 1);
		//All the points on one face
		m_VertexCoords = std::vector<glm::vec3>();
		for (int face = 0; face < 6; face++)
		{
			for (int y = 0; y < divisions + 1; y++) {
				for (int x = 0; x < divisions + 1; x++) {
					float xf = ((float)x / (float)divisions) - 0.5f;
					float yf = ((float)y / (float)divisions) - 0.5f;
					m_VertexCoords.push_back(cubeRotation[face] * glm::normalize(glm::vec3(xf, yf, 0.5f)) * radius);
				}
			}
		}
	}

	void RenderShapeSphere::Rebuild()
	{
		Build(m_Radius, m_Divisions);
	}
}