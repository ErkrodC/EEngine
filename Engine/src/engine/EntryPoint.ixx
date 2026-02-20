import EEngine.Core;
import EEngine.Application;

int main() {
	EEngine::Log::Initialize();
	EEngine::Log::CoreTrace("Initialized Logging.");
	EEngine::Log::Trace("Initialized Logging.");

	auto app = EEngine::CreateApplication();
	app->Run();
	delete app;

	return 0;
}