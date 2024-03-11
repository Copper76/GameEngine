#pragma once

#ifdef FS_PLATFORM_WINDOWS

extern Fengshui::Application* Fengshui::CreateApplication();

int main(int argc, char** argv)
{
	//Logging system doesn't work at the moment
	Fengshui::Log::Init();
	//FS_INFO("Client Log!");

	auto app = Fengshui::CreateApplication();
	app->Run();
	//delete app;

	return 0;
}
#endif