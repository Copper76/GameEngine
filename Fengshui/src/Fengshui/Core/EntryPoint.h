#pragma once

#ifdef FS_PLATFORM_WINDOWS

extern Fengshui::Application* Fengshui::CreateApplication();

//The main function
int main(int argc, char** argv)
{
	//Inmitialise logging for the engine
	Fengshui::Log::Init();

	//Creates the application that will run and enters the running cycle
	auto app = Fengshui::CreateApplication();
	app->Run();

	return 0;
}
#endif