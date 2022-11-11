#include "EEngine.hpp"

class ExampleLayer : public EEngine::Layer {
public:
	ExampleLayer()
		: Layer("Example") {}

	void OnUpdate() override {
		EE_INFO("ExampleLayer::Update");
	}

	void OnEvent(EEngine::Event& event) override {
		EE_TRACE(event);
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