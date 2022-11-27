#pragma once

#include "Application.hpp"

#ifdef EE_PLATFORM_WINDOWS
	int main(int argc, char** argv) {
		EEngine::Log::Initialize();
		EE_CORE_TRACE("Initialized Logging.");
		EE_TRACE("Initialized Logging.");

		auto app = EEngine::CreateApplication();
		app->Run();
		delete app;

		return 0;
	}
#endif