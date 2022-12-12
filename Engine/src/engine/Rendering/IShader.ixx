export module EEngine.Rendering:IShader;
import EEngine.Core;
import EEngine.std.core;

export namespace EEngine {
	class IShader {
	public:
		virtual ~IShader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;
	};
} // EEngine
