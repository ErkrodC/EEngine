module;
#include <Profiling/Profiling.hpp>

export module Runner:RunnerLayer3D;
import EEngine;
import EEngine.Profiling;

using namespace EEngine;
using namespace Rendering;

// ER TODO this class is very WIP, might not make a lot of sense what it's doing right now.
export class RunnerLayer3D : public Layer {
public:
	RunnerLayer3D(RendererAPI& rendererAPI, Renderer& renderer, Input& input)
		: Layer("RunnerLayer3D")
		, m_Input(input)
		, m_CameraController(16.0f / 9.0f, m_Input)
		, m_RendererAPI(rendererAPI)
		, m_Renderer(renderer)
	{
		Shared<Shader> textureShader;
		if (m_RendererAPI.TryGetOrLoadShader("assets/shaders/Texture.glsl", textureShader)) {
			// m_Texture = m_RendererAPI.CreateTexture2D("assets/textures/test.png");
			uint32_t whiteTextureData = 0xffffffff;
			m_Texture = m_RendererAPI.CreateTexture2D(1, 1, &whiteTextureData, sizeof(uint32_t));
		} else {
			Log::CoreCritical("Failed to load texture shader.");
			throw std::runtime_error("Failed to load texture shader.");
		}
	}

	void OnUpdate(Timestep timestep) override {
		EE_PROFILE_FUNCTION();

		m_RendererAPI.Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

		m_CameraController.OnUpdate(timestep);
		HandleTriMovement(timestep);

		Math::mat4 projectionView = m_CameraController.GetCamera().GetProjectionViewMatrix();
		
		m_Renderer.BeginScene(projectionView);
		m_Renderer.DrawQuad(m_TriPos, { 1.0f, 1.0f }, m_Texture);
		m_Renderer.DrawQuad({ 0.5f, 0.5f }, { 0.5f, 0.5f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		m_Renderer.EndScene();
	}

	void OnIMGUIRender() override {

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
	Shared<Texture2D> m_Texture;
	Math::vec3 m_TriPos{};
};
