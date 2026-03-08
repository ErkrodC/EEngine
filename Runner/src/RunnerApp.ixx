export module Runner:RunnerApp;
import :RunnerLayer2D;
import :RunnerLayer3D;
import EEngine;

using namespace EEngine;

class RunnerApp : public Application {
public:
	RunnerApp() {
		PushLayer(MakeUnique<RunnerLayer3D>(*m_RendererAPI, *m_Renderer, *m_Input));
		//PushLayer(MakeUnique<RunnerLayer2D>(*m_RendererAPI, *m_Renderer, *m_Input));
	}
};

namespace EEngine {
	Unique<Application> CreateApplication() { return MakeUnique<RunnerApp>(); }
}
