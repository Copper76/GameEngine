#include "fspch.h"
#include "RenderShapeCube.h"

namespace Fengshui
{
	RenderShapeCube::RenderShapeCube()
	{
		std::array<std::array<glm::vec3, 4>, 6> cubeVertPos;
		cubeVertPos[0] = { glm::vec3{ -0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, -0.5f }, { -0.5f, 0.5f, -0.5f } };
		cubeVertPos[1] = { glm::vec3{ 0.5f, -0.5f, -0.5f }, { 0.5f, -0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, -0.5f } };
		cubeVertPos[2] = { glm::vec3{ 0.5f, -0.5f, 0.5f }, { -0.5f, -0.5f, 0.5f } , { -0.5f, 0.5f, 0.5f }, { 0.5f, 0.5f, 0.5f } };
		cubeVertPos[3] = { glm::vec3{ -0.5f, -0.5f, 0.5f }, { -0.5f, -0.5f, -0.5f }, { -0.5f, 0.5f, -0.5f }, { -0.5f, 0.5f, 0.5f } };
		cubeVertPos[4] = { glm::vec3{ -0.5f, 0.5f, -0.5f }, { 0.5f, 0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f }, { -0.5f, 0.5f, 0.5f } };
		cubeVertPos[5] = { glm::vec3{ -0.5f, -0.5f, 0.5f }, { 0.5f, -0.5f, 0.5f }, { 0.5f, -0.5f, -0.5f }, { -0.5f, -0.5f, -0.5f } };
		for (uint32_t face = 0; face < 6; face++)
		{
			for (uint32_t i = 0; i < 4; i++)
			{
				m_VertexCoords.push_back(cubeVertPos[face][i]);
			}
		}
	}
}
