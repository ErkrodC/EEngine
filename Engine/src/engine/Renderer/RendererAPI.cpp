#include <Platform/OpenGL/OpenGLRendererAPI.hpp>
#include "RendererAPI.hpp"

namespace EEngine {
	IRendererAPI* RendererAPI::s_RendererAPI = new OpenGLRendererAPI();
} // EEngine