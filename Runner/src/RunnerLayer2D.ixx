module;
#include <Profiling/Profiling.hpp>

export module Runner:RunnerLayer2D;
import EEngine;
import EEngine.Profiling;

using namespace EEngine;

export class RunnerLayer2D : public Layer {
public:
	RunnerLayer2D() : Layer("RunnerLayer2D"), m_CameraController(16.0f / 9.0f) {

	}

	void OnAttach() override {
		m_Texture = RendererAPI::CreateTexture2D("assets/textures/test.png");
	}

	void OnDetach() override {

	}

;	void OnUpdate(Timestep timestep) override {
		EE_PROFILE_FUNCTION();

		m_CameraController.OnUpdate(timestep);

		RendererAPI::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

		HandleTriMovement(timestep);

		Renderer2D::BeginScene(m_CameraController.GetCamera()); {
			Renderer2D::DrawQuad({ -1.0f, 0.0f }, {0.8f, 0.8f}, m_SquareColor);
			//EEngine::Renderer2D::DrawQuad({ 0.5f, -0.5f }, {0.5f, 0.75f}, m_SquareColor);
			Renderer2D::DrawQuad({ 0.5f, -0.5f }, {0.5f, 0.75f}, m_Texture);
		} Renderer2D::EndScene();

		// ER TODO usually executed on a separate thread
		//EEngine::Renderer::Flush();
	}

	void OnIMGUIRender() override {
		Editor::Begin("Settings");
		Editor::ColorEdit4("Square Color", Math::value_ptr(m_SquareColor));

		Editor::Separator();
		Editor::Text("Profiler");
		Editor::Separator();

		auto& profiler = Profiling::Profiler::Get();
		auto& currentFrame = profiler.GetCurrentFrame();

		// FPS and frame time
		float avgFPS = profiler.GetAverageFPS(60);
		Editor::Text("FPS: %.1f (%.2fms)", avgFPS, currentFrame.TotalFrameTime);

		Editor::Separator();

		// Individual profile results
		for (const auto& result : currentFrame.Results) {
			float percentage = (result.DurationMs / currentFrame.TotalFrameTime) * 100.0f;
			Editor::Text("%s: %.3fms (%.1f%%) [%d calls]",
				result.Name,
				result.DurationMs,
				percentage,
				result.CallCount);
		}

		Editor::End();
	}

	void OnEvent(Event& event) override {
		m_CameraController.OnEvent(event);
	}

	void HandleTriMovement(Timestep timestep) {
		float deltaDist = 1.0f * timestep.GetSeconds();

		if (Input::IsKeyPressed(KeyCode::I)) {
			m_TriPos.y += deltaDist;
		}

		if (Input::IsKeyPressed(KeyCode::J)) {
			m_TriPos.x -= deltaDist;
		}

		if (Input::IsKeyPressed(KeyCode::K)) {
			m_TriPos.y -= deltaDist;
		}

		if (Input::IsKeyPressed(KeyCode::L)) {
			m_TriPos.x += deltaDist;
		}
	}
private:
	CameraController m_CameraController;

	// ER TEMP
	Math::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	Math::vec3 m_TriPos{};
	Shared<ITexture2D> m_Texture;
};