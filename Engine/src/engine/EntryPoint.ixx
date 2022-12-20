#include "Core/Core.hpp"
import EEngine.Core;
import EEngine.Application;

#ifdef EE_PLATFORM_WINDOWS
int main(int argc, char** argv) {
	EEngine::Input::SetInputInstance(new EEngine::WindowsInput());
	EEngine::Log::Initialize();
	EEngine::Log::CoreTrace("Initialized Logging.");
	EEngine::Log::Trace("Initialized Logging.");

	auto app = EEngine::CreateApplication();
	app->Run();
	delete app;

	return 0;
}
#endif
