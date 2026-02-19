module;
#include <Profiling/Profiling.hpp>

export module Runner:RunnerLayer2D;
import EEngine;
import EEngine.Profiling;

export class RunnerLayer2D : public EEngine::Layer {
public:
	RunnerLayer2D() : Layer("RunnerLayer2D"), m_CameraController(16.0f / 9.0f) {

	}

	void OnAttach() override {
		m_Texture = EEngine::RendererAPI::CreateTexture2D("assets/textures/test.png");
	}

	void OnDetach() override {

	}

;	void OnUpdate(EEngine::Timestep timestep) override {
		EE_PROFILE_FUNCTION();

		m_CameraController.OnUpdate(timestep);

		EEngine::RendererAPI::Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

		HandleTriMovement(timestep);

		EEngine::Renderer2D::BeginScene(m_CameraController.GetCamera()); {
			EEngine::Renderer2D::DrawQuad({ -1.0f, 0.0f }, {0.8f, 0.8f}, m_SquareColor);
			//EEngine::Renderer2D::DrawQuad({ 0.5f, -0.5f }, {0.5f, 0.75f}, m_SquareColor);
			EEngine::Renderer2D::DrawQuad({ 0.5f, -0.5f }, {0.5f, 0.75f}, m_Texture);
		} EEngine::Renderer2D::EndScene();

		// ER TODO usually executed on a separate thread
		//EEngine::Renderer::Flush();
	}

	void OnIMGUIRender() override {
		EEngine::Editor::Begin("Settings");
		EEngine::Editor::ColorEdit4("Square Color", EEngine::Math::value_ptr(m_SquareColor));

		EEngine::Editor::Separator();
		EEngine::Editor::Text("Profiler");
		EEngine::Editor::Separator();

		auto& profiler = EEngine::Profiling::Profiler::Get();
		auto& currentFrame = profiler.GetCurrentFrame();

		// FPS and frame time
		float avgFPS = profiler.GetAverageFPS(60);
		EEngine::Editor::Text("FPS: %.1f (%.2fms)", avgFPS, currentFrame.TotalFrameTime);

		EEngine::Editor::Separator();

		// Individual profile results
		for (const auto& result : currentFrame.Results) {
			float percentage = (result.DurationMs / currentFrame.TotalFrameTime) * 100.0f;
			EEngine::Editor::Text("%s: %.3fms (%.1f%%) [%d calls]",
				result.Name,
				result.DurationMs,
				percentage,
				result.CallCount);
		}

		EEngine::Editor::End();
	}

	void OnEvent(EEngine::Event& event) override {
		m_CameraController.OnEvent(event);
	}

	void HandleTriMovement(EEngine::Timestep timestep) {
		float deltaDist = 1.0f * timestep.GetSeconds();

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::I)) {
			m_TriPos.y += deltaDist;
		}

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::J)) {
			m_TriPos.x -= deltaDist;
		}

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::K)) {
			m_TriPos.y -= deltaDist;
		}

		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::L)) {
			m_TriPos.x += deltaDist;
		}
	}
private:
	EEngine::CameraController m_CameraController;

	// ER TEMP
	EEngine::Math::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	EEngine::Math::vec3 m_TriPos{};
	EEngine::Ref<EEngine::ITexture2D> m_Texture;
};