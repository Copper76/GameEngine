#pragma once

namespace Fengshui
{
	struct Triangle
	{
		int a, b, c;
	};

	struct Edge {
		int a;
		int b;

		bool operator == (const Edge& rhs) const {
			return ((a == rhs.a && b == rhs.b) || (a == rhs.b && b == rhs.a));
		}
	};
}