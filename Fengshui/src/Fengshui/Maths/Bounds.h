//
//	Bounds.h
//
#pragma once
#include <math.h>
#include <assert.h>
#include "Vector.h"
#include <vector>
#include <glm/glm.hpp>

namespace Fengshui
{
	/*
	====================================================
	Bounds
	====================================================
	*/
	class Bounds {
	public:
		Bounds() { Clear(); }
		Bounds( const Bounds & rhs ) : mins( rhs.mins ), maxs( rhs.maxs ) {}
		const Bounds & operator = ( const Bounds & rhs );
		~Bounds() {}

		void Clear() { mins = glm::vec3( 1e6 ); maxs = glm::vec3( -1e6 ); }
		bool DoesIntersect( const Bounds & rhs ) const;
		void Expand( const glm::vec3* pts, const int num );
		void Expand( const glm::vec3& rhs );
		void Expand( const Bounds & rhs );

		float WidthX() const { return maxs.x - mins.x; }
		float WidthY() const { return maxs.y - mins.y; }
		float WidthZ() const { return maxs.z - mins.z; }

	public:
		glm::vec3 mins;
		glm::vec3 maxs;
	};
}