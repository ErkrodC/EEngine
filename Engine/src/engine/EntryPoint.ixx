#include "Profiling/Profiling.hpp"
import EEngine.Core;
import EEngine.Application;
import EEngine.Profiling;

#ifdef EE_PLATFORM_WINDOWS
int main(int argc, char** argv) {
	EEngine::Input::SetInputInstance(new EEngine::WindowsInput());
	EEngine::Log::Initialize();
	EEngine::Log::CoreTrace("Initialized Logging.");
	EEngine::Log::Trace("Initialized Logging.");

	EE_PROFILE_BEGIN_SESSION("Runtime", "EEngineProfile.json");
	auto app = EEngine::CreateApplication();
	app->Run();
	delete app;
	EE_PROFILE_END_SESSION();

	return 0;
}
#endif
