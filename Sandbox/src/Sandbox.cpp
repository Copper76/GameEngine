#include <Fengshui.h>

//External includes
#include <imgui.h>

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

	void OnImGuiRender() override
	{
		//ImGui::Begin("TEST");
		//ImGui::Text("Hello World");
		//ImGui::End();
	}
};

class Sandbox : public Fengshui::Application
{
public:
	Sandbox() : Application()
	{
		//ExampleLayer* layer = new ExampleLayer();
		//PushLayer(layer);
	}

	~Sandbox()
	{

	}
};

Fengshui::Application* Fengshui::CreateApplication()
{
	return new Sandbox();
}