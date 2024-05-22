//
//	ShapeBase.h
//
#pragma once
#include "../../Math/Vector.h"
#include "../../Math/Matrix.h"
#include "../../Math/Bounds.h"
#include <vector>

namespace Fengshui
{
	struct Transform;
	struct Collider;
	/*
	====================================================
	Shape
	====================================================
	*/
	class Shape {
	public:
		//virtual void Build(const Vec3* pts, const int num) {}
		virtual void Build(const glm::vec3* pts, const int num) {}
		//virtual Vec3 Support(const Vec3& dir, const Vec3& pos, const Quat& orient, const float bias) const = 0;
		virtual glm::vec3 Support(const glm::vec3& dir, const Transform transform, const Collider collider, const float bias) const = 0;

		//virtual Mat3 InertiaTensor() const = 0;
		virtual glm::mat3 InertiaTensor(const Transform transform, const Collider collider) const = 0;

		//virtual Bounds GetBounds( const Vec3 & pos, const Quat & orient ) const = 0;
		virtual Bounds GetBounds(const Transform transform, const Collider collider) const = 0;
		virtual Bounds GetBounds(const glm::vec3 scale, const Collider collider) const = 0;

		//virtual Vec3 GetCenterOfMass() const { return m_centerOfMass; }
		virtual glm::vec3 GetCenterOfMass(const glm::vec3 offset) const { return m_centerOfMass + offset; }

		enum shapeType_t {
			SHAPE_SPHERE,
			SHAPE_BOX,
			SHAPE_CONVEX,
		};
		virtual shapeType_t GetType() const = 0;

		//virtual float FastestLinearSpeed( const Vec3& angularVelocity, const Vec3& dir ) const { return 0.0f; }
		virtual float FastestLinearSpeed(const glm::vec3& angularVelocity, const glm::vec3& dir) const { return 0.0f; }

	protected:
		glm::vec3 m_centerOfMass;
	};
}
