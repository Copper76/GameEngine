#pragma once

#ifdef FS_PLATFORM_WINDOWS
extern Fengshui::Application* Fengshui::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Fengshui::CreateApplication();
	app->Run();
	delete app;

	return 0;
}
#endif