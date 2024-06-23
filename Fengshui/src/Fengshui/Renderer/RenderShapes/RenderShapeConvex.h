#pragma once
#include "Fengshui/Renderer/RenderShapes/RenderShapeBase.h"

namespace Fengshui
{
	class RenderShapeConvex : public RenderShape
	{
	public:
		explicit RenderShapeConvex(const glm::vec3* pts, const int num)
		{
			Build(pts, num);
		}

		virtual void Build(const glm::vec3* pts, const int num) override;

		void Rebuild(const std::vector<glm::vec3> pts, bool removeUnused = false);

		virtual ShapeType GetType() const override { return ShapeType::SHAPE_CONVEX;  }

		virtual std::string GetDisplayName() const override { return "Convex"; }

		inline void SetVertexNum(int vertexNum) { m_VertexNum = vertexNum; }

		inline const std::vector<glm::vec3> GetVertexCoords() const override { return m_VertexCoords; }
		inline const std::vector<Triangle> GetTris() const { return m_Tris; }
		inline const int GetVertexNum() const { return m_VertexNum; }

		inline std::vector<glm::vec3>& GetVertexCoords() { return m_VertexCoords; }
		inline std::vector<Triangle>& GetTris() { return m_Tris; }
		inline int& GetVertexNum() { return m_VertexNum; }

	private:
		std::vector<glm::vec3> m_VertexCoords;
		std::vector<Triangle> m_Tris;
		int m_VertexNum;
	};
}