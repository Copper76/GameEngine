#include <Fengshui.h>

class ExampleLayer : public Fengshui::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{

	}

	void OnUpdate() override
	{
		FS_INFO("ExampleLayer::Update");
	}

	void OnEvent(Fengshui::Event& e) override
	{
		FS_INFO(e);
	}

	void OnAttach() override
	{

	}

	void OnDetach() override
	{

	}
};

class Sandbox : public Fengshui::Application
{
public:
	Sandbox() : Application()
	{
		//PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Fengshui::Application* Fengshui::CreateApplication()
{
	return new Sandbox();
}