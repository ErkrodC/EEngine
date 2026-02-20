export module Runner:RunnerApp;
import :RunnerLayer2D;
import :RunnerLayer3D;
import EEngine;

class RunnerApp : public EEngine::Application {
public:
	RunnerApp() {
		//PushLayer(EEngine::MakeRef<RunnerLayer3D>());
		PushLayer(EEngine::MakeRef<RunnerLayer2D>());
	}

	~RunnerApp() override = default;
};

EEngine::Application* EEngine::CreateApplication() {
	return new RunnerApp();
}
