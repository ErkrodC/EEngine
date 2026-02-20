import EEngine.Core;
import EEngine.Application;

using namespace EEngine;

int main() {
	Log::Initialize();
	Log::CoreTrace("Initialized Logging.");
	Log::Trace("Initialized Logging.");

	CreateApplication()->Run();

	return 0;
}