#include "EEngine.hpp"
#include "KeyCode.hpp"
#include "imgui.h"

class ExampleLayer : public EEngine::Layer {
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override {
		if (EEngine::Input::IsKeyPressed(EEngine::KeyCode::Tab)) {
			EE_TRACE("TAb key is pressed (poll)!");
		}
	}

	void OnIMGUIRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(EEngine::Event& event) override {
		//EE_TRACE(event);
	}
};

class Runner : public EEngine::Application {
public:
	Runner() {
		PushLayer(new ExampleLayer());
	}

	~Runner() {}
};

EEngine::Application* EEngine::CreateApplication() {
	return new Runner();
}