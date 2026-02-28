export module EEngine.Rendering:API;
import EEngine.Core;
import EEngine.Standard;

export namespace EEngine::Rendering {
	enum class API {
		DirectX,
		OpenGL,
		Vulkan
	};

	constexpr API g_API = API::OpenGL;
	constexpr uint32_t MAX_QUADS = 10000;

	constexpr std::string GetRendererAPIString() {
		if constexpr (g_API == API::OpenGL) {
			return "OpenGL";
		} else if constexpr (g_API == API::DirectX) {
			return "DirectX";
		} else if constexpr (g_API == API::Vulkan) {
			return "Vulkan";
		} else {
			Log::CoreCritical("Unknown rendering API.");
			throw std::runtime_error("Unknown rendering API.");
		}
	}
}
