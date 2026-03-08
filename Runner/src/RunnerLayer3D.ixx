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
		if (!m_RendererAPI.TryGetOrLoadShader("assets/shaders/Texture.glsl", textureShader)) {
			Log::CoreCritical("Failed to load texture shader.");
			throw std::runtime_error("Failed to load texture shader.");
		}
	}

	void OnUpdate(Timestep timestep) override {
		EE_PROFILE_FUNCTION();

		m_CameraController.OnUpdate(timestep);
		m_RendererAPI.Clear({ 0.1f, 0.1f, 0.1f, 1.0f });

		m_Rotation += timestep.GetSeconds();

		m_Renderer.BeginScene(m_CameraController.GetCamera().GetProjection());

		// Three cubes: red center, green left, blue right
		Math::quat spin = Math::quat(Math::vec3(m_Rotation * 0.5f, m_Rotation, 0.0f));
		m_Renderer.DrawCube({0.0f,0.0f,-3.0f}, spin, {1.0f,1.0f,1.0f},{0.8f,0.2f,0.3f,1.0f});
		m_Renderer.DrawCube({-2.5f,  0.0f, -3.0f}, spin, {0.75f, 0.75f, 0.75f}, {0.2f, 0.8f, 0.3f, 1.0f});
		m_Renderer.DrawCube({ 2.5f,  0.0f, -3.0f}, spin, {1.0f, 1.0f, 1.0f}, {0.2f, 0.3f, 0.8f, 1.0f});

		m_Renderer.EndScene();
	}

	void OnIMGUIRender() override {}

	void OnEvent(Event& event) override {
		m_CameraController.OnEvent(event);
	}

private:
	Input& m_Input;
	CameraController m_CameraController;
	RendererAPI& m_RendererAPI;
	Renderer& m_Renderer;

	float_t m_Rotation = 0.0f;
};
