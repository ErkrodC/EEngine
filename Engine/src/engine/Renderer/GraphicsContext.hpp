#pragma once

namespace EEngine {
	class GraphicsContext {
	public:
		virtual void Initialize() = 0;
		virtual void SwapBuffers() = 0;
	};
} // EEngine
