module;
#include <Profiling/Profiling.hpp>

export module Runner:RunnerLayer2D;
import EEngine;
import EEngine.Profiling;

using namespace EEngine;
using namespace Rendering;

export class RunnerLayer2D : public Layer {
public:
	RunnerLayer2D(RendererAPI& context, Renderer& renderer, Input& input)
		: Layer("RunnerLayer2D")
		, m_Input(input)
		, m_CameraController(16.0f / 9.0f, m_Input)
		, m_RendererAPI(context)
		, m_Renderer(renderer)
		, m_Scene(MakeUnique<Scene>()
	) {
		SceneEntity mainCameraEntity = m_Scene->CreateEntity("Main Camera");
		CameraComponent& mainCamera = mainCameraEntity.AddComponent<CameraComponent>();
		mainCamera.Projection = Math::perspective(Math::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
		mainCamera.IsPrimary = true;
	}

	void OnAttach() override {
		m_Texture = m_RendererAPI.CreateTexture2D("assets/textures/test.png");
	}

	void OnDetach() override {

	}

	void OnUpdate(Timestep timestep) override {
		EE_PROFILE_FUNCTION();

		m_CameraController.OnUpdate(timestep);

		m_RendererAPI.Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

		HandleTriMovement(timestep);

		m_Scene->OnUpdate(timestep);
		m_Scene->OnRender(m_Renderer);
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
		float_t avgFPS = profiler.GetAverageFPS(60);
		Editor::Text("FPS: %.1f (%.2fms)", avgFPS, currentFrame.TotalFrameTime);

		Editor::Separator();

		// Individual profile results
		for (const auto& result : currentFrame.Results) {
			float_t percentage = (result.DurationMs / currentFrame.TotalFrameTime) * 100.0f;
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
		float_t deltaDist = 1.0f * timestep.GetSeconds();

		if (m_Input.IsKeyPressed(KeyCode::I)) {
			m_TriPos.y += deltaDist;
		}

		if (m_Input.IsKeyPressed(KeyCode::J)) {
			m_TriPos.x -= deltaDist;
		}

		if (m_Input.IsKeyPressed(KeyCode::K)) {
			m_TriPos.y -= deltaDist;
		}

		if (m_Input.IsKeyPressed(KeyCode::L)) {
			m_TriPos.x += deltaDist;
		}
	}
private:
	Input& m_Input;
	CameraController m_CameraController;
	RendererAPI& m_RendererAPI;
	Renderer& m_Renderer;
	Unique<Scene> m_Scene;

	// ER TEMP
	Math::vec4 m_SquareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	Math::vec3 m_TriPos{};
	Shared<Texture2D> m_Texture;
};