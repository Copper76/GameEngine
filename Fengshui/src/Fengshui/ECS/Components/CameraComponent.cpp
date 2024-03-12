#include "fspch.h"
#include "CameraComponent.h"

namespace Fengshui
{
	CameraComponent::CameraComponent()
	{
		m_Camera = std::make_shared<OrthographicCamera>(-1.0f, 1.0f, -1.0f, 1.0f);
	}
}