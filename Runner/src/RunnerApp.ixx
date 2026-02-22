export module Runner:RunnerApp;
import :RunnerLayer2D;
import :RunnerLayer3D;
import EEngine;

using namespace EEngine;

class RunnerApp : public Application {
public:
	RunnerApp() {
		//PushLayer(MakeRef<RunnerLayer3D>());
		PushLayer(MakeUnique<RunnerLayer2D>());
	}
};

namespace EEngine {
	Unique<Application> CreateApplication() { return MakeUnique<RunnerApp>(); }
}
