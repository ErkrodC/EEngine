export module EEngine.Rendering:Interfaces;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;
import :Buffers;

using namespace EEngine;

export namespace EEngine::Rendering {
	enum class API {
		DirectX,
		OpenGL,
		Vulkan
	};

	constexpr API g_API = API::OpenGL;

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

	#define DECLARE_API_TYPE(BaseName) \
		class DirectX##BaseName; class OpenGL##BaseName; class Vulkan##BaseName; \
		using BaseName = std::conditional_t<g_API == API::DirectX, \
			DirectX##BaseName, \
			std::conditional_t<g_API == API::OpenGL, \
				OpenGL##BaseName, \
				Vulkan##BaseName \
			> \
		>

	DECLARE_API_TYPE(Shader);
	DECLARE_API_TYPE(VertexArray);
	DECLARE_API_TYPE(Texture2D);
	DECLARE_API_TYPE(IndexBuffer);
	DECLARE_API_TYPE(VertexBuffer);

	#undef DECLARE_API_TYPE
}
