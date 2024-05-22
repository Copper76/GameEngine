//
//	ShapeConvex.h
//
#pragma once
#include "ShapeBase.h"

namespace Fengshui
{
	struct tri_t {
		int a;
		int b;
		int c;
	};

	struct edge_t {
		int a;
		int b;

		bool operator == (const edge_t& rhs) const {
			return ((a == rhs.a && b == rhs.b) || (a == rhs.b && b == rhs.a));
		}
	};

	//void BuildConvexHull( const std::vector< Vec3 > & verts, std::vector< Vec3 > & hullPts, std::vector< tri_t > & hullTris );
	void BuildConvexHull(const std::vector<glm::vec3>& verts, std::vector<glm::vec3>& hullPts, std::vector< tri_t >& hullTris);

	/*
	====================================================
	ShapeConvex
	====================================================
	*/
	class ShapeConvex : public Shape {
	public:
		/*explicit ShapeConvex( const Vec3 * pts, const int num ) {
			Build( pts, num );
		}*/
		explicit ShapeConvex(const glm::vec3* pts, const int num) {
			Build(pts, num);
		}

		//void Build( const Vec3 * pts, const int num ) override;
		void Build(const glm::vec3* pts, const int num) override;

		//Vec3 Support( const Vec3 & dir, const Vec3 & pos, const Quat & orient, const float bias ) const override;
		glm::vec3 Support(const glm::vec3& dir, const Transform transform, const Collider collider, const float bias) const override;

		//Mat3 InertiaTensor() const override { return m_inertiaTensor; }
		glm::mat3 InertiaTensor(const Transform transform, const Collider collider) const override;

		//Bounds GetBounds( const Vec3 & pos, const Quat & orient ) const override;
		Bounds GetBounds(const Transform transform, const Collider collider) const override;
		Bounds GetBounds(const glm::vec3 scale, const Collider collider) const override;

		//float FastestLinearSpeed( const Vec3 & angularVelocity, const Vec3 & dir ) const override;
		float FastestLinearSpeed(const glm::vec3& angularVelocity, const glm::vec3& dir) const override;

		shapeType_t GetType() const override { return SHAPE_CONVEX; }

	public:
		std::vector<glm::vec3> m_points;
		Bounds m_bounds;
		glm::mat3 m_inertiaTensor;
	};
}