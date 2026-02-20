import EEngine.Core;
import EEngine.Application;

int main(int argc, char** argv) {
	EEngine::Log::Initialize();
	EEngine::Log::CoreTrace("Initialized Logging.");
	EEngine::Log::Trace("Initialized Logging.");

	auto app = EEngine::CreateApplication();
	app->Run();
	delete app;

	return 0;
}