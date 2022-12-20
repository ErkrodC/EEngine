export module EEngine.Rendering:IShader;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;

export namespace EEngine {
	class IShader {
	public:
		virtual ~IShader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int32_t value) = 0;

		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const Math::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const Math::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const Math::vec4& value) = 0;

		virtual void SetMat3(const std::string& name, const Math::mat3& value) = 0;
		virtual void SetMat4(const std::string& name, const Math::mat4& value) = 0;

		virtual const std::string& GetName() const = 0;
	};
} // EEngine
