#include "fspch.h"
#include "Layer.h"

namespace Fengshui
{
	Layer::Layer(const std::string& debugName, bool isActive) : m_DebugName(debugName), m_IsActive(isActive) {}

	Layer::~Layer()
	{

	}
}
