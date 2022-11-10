#pragma once

#include "Core.hpp"

namespace EEngine {
	class Application {
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// to be defined by consumer
	Application* CreateApplication();
} // Engine
