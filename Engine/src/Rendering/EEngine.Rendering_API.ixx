export module EEngine.Rendering:API;
import EEngine.Core;
import EEngine.Standard;

export namespace EEngine::Rendering {
	enum class API { DirectX, OpenGL, Vulkan };

#if EE_GFX_DIRECTX
	constexpr API g_API = API::DirectX;
#elif EE_GFX_OPENGL
	constexpr API g_API = API::OpenGL;
#elif EE_GFX_VULKAN
	constexpr API g_API = API::Vulkan;
#endif

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
