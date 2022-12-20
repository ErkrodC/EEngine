module;
#include <cstdint>

export module EEngine.Rendering:ITexture;
import EEngine.Standard;

export namespace EEngine {
	class ITexture {
	public:
		virtual ~ITexture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind() const = 0;
	};

	class ITexture2D : public ITexture { };
} // EEngine
