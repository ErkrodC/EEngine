export module Runner:RunnerApp;
import :RunnerLayer2D;
import :RunnerLayer3D;
import EEngine;

using namespace EEngine;

class RunnerApp : public Application {
public:
	RunnerApp() {
		//PushLayer(MakeRef<RunnerLayer3D>(*m_RendererAPI, *m_Renderer));
		PushLayer(MakeUnique<RunnerLayer2D>(*m_RendererAPI, *m_Renderer));
	}
};

namespace EEngine {
	Unique<Application> CreateApplication() { return MakeUnique<RunnerApp>(); }
}
