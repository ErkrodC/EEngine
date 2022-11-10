#include "EEngine.hpp"

class Runner : public EEngine::Application {
public:
	Runner() {}
	~Runner() {}
};

EEngine::Application* EEngine::CreateApplication() {
	return new Runner();
}