#pragma once

#ifdef FS_PLATFORM_WINDOWS
extern Fengshui::Application* Fengshui::CreateApplication();

int main(int argc, char** argv)
{
	Fengshui::Log::Init();
	Fengshui::Log::GetEngineLogger()->warn("Initialized Log!");
	Fengshui::Log::GetClientLogger()->info("Client Log!");

	auto app = Fengshui::CreateApplication();
	app->Run();
	delete app;

	return 0;
}
#endif