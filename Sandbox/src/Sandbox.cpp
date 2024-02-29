#include <Fengshui.h>

class Sandbox : public Fengshui::Application
{
public:
	Sandbox()
	{
	}

	~Sandbox()
	{

	}
};

Fengshui::Application* Fengshui::CreateApplication()
{
	return new Sandbox();
}