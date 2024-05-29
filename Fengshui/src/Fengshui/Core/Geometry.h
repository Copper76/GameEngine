#pragma once
#include "Fengshui/Logging/Log.h"

#include "Fengshui/Core/Triangularisation/Delaunay.h"
#include "Fengshui/Core/Triangularisation/FurthestPoint.h"

namespace Fengshui
{
	enum class TriMethod
	{
		FurthestPoint, Delaunay
	};
	static TriMethod triMethod;

	static void BuildConvexHull(const std::vector<glm::vec3>& verts, std::vector<glm::vec3>& hullPts, std::vector<Triangle>& hullTris) {
		if (verts.size() < 4) {
			return;
		}

		switch (triMethod)
		{
		case TriMethod::FurthestPoint:
			BuildConvexHullFurthestPoint(verts, hullPts, hullTris);
			break;
		default:
			FS_ENGINE_ERROR("Undefined Triangularisation method");
			break;
		}
	}

	static glm::mat3 CalculateInertiaTensorTetrahedron(const std::vector<glm::vec3>& pts, const std::vector<Triangle>& tris, const glm::vec3& cm) 
	{
		switch (triMethod)
		{
		case TriMethod::FurthestPoint:
			return CalculateInertiaTensorTetrahedronFurthestPoint(pts, tris, cm);
			break;
		default:
			FS_ENGINE_ERROR("Undefined Triangularisation method");
			return glm::mat3(0.0f);
			break;
		}
	}

	static glm::vec3 CalculateCenterOfMassTetrahedron(const std::vector<glm::vec3>& pts, const std::vector<Triangle>& tris)
	{
		switch (triMethod)
		{
		case TriMethod::FurthestPoint:
			return CalculateCenterOfMassTetrahedronFurthestPoint(pts, tris);
			break;
		default:
			FS_ENGINE_ERROR("Undefined Triangularisation method");
			return glm::vec3(0.0f);
			break;
		}
	}
}