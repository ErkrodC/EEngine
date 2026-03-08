export module EEngine.Rendering:Shader;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;
import :API;

export namespace EEngine::Rendering {
	std::string GetNameFromPath(const std::string& path);

	// ============================================================================
	// Shader Type Alias
	// ============================================================================
	class DirectXShader;
	class VulkanShader;
	class OpenGLShader;
	using Shader = std::conditional_t<g_API == API::DirectX,
		DirectXShader,
		std::conditional_t<g_API == API::OpenGL,
			OpenGLShader,
			VulkanShader
		>
	>;

	// ============================================================================
	// OpenGLShader Implementation
	// ============================================================================
	class OpenGLShader {
	public:
		static Expected<Shared<OpenGLShader>, std::string> Create(
			const std::string& name,
			const std::string& vertexSource,
			const std::string& fragmentSource
		);
		static Expected<Shared<OpenGLShader>, std::string> Create(const std::string& path);

		OpenGLShader(const std::string& name, uint32_t rendererID);
		~OpenGLShader();
		OpenGLShader(const OpenGLShader& other) = delete;
		OpenGLShader& operator=(const OpenGLShader& other) = delete;

		OpenGLShader(OpenGLShader&& other) noexcept;
		OpenGLShader& operator=(OpenGLShader&& other) noexcept;

		void Bind() const;
		void Unbind() const;

		void SetInt(const std::string& name, int32_t value) const;
		void SetFloat(const std::string& name, float_t value) const;
		void SetFloat2(const std::string& name, const Math::vec2& values) const;
		void SetFloat3(const std::string& name, const Math::vec3& values) const;
		void SetFloat4(const std::string& name, const Math::vec4& values) const;
		void SetMat3(const std::string& name, const Math::mat3& matrix) const;
		void SetMat4(const std::string& name, const Math::mat4& matrix) const;

		const std::string& GetName() const { return m_Name; }
	private:
		uint32_t m_RendererID = 0;
		std::string m_Name;
		mutable std::unordered_map<std::string, int32_t> m_UniformLocationCache;

		static Expected<uint32_t, std::string> CompileShaders(const std::unordered_map<uint32_t, std::string>& sources);
		int32_t GetUniformLocation(const std::string& name) const;
	};

	// ============================================================================
	// Shader Concept and Static Assert
	// ============================================================================
	template<typename T>
	concept ShaderConcept = requires(
		T shader,
		std::string name,
		int32_t intValue,
		float_t floatValue,
		Math::vec2 v2,
		Math::vec3 v3,
		Math::vec4 v4,
		Math::mat3 m3,
		Math::mat4 m4
	) {
		{ shader.Bind() } -> std::same_as<void>;
		{ shader.Unbind() } -> std::same_as<void>;
		{ shader.SetInt(name, intValue) } -> std::same_as<void>;
		{ shader.SetFloat(name, floatValue) } -> std::same_as<void>;
		{ shader.SetFloat2(name, v2) } -> std::same_as<void>;
		{ shader.SetFloat3(name, v3) } -> std::same_as<void>;
		{ shader.SetFloat4(name, v4) } -> std::same_as<void>;
		{ shader.SetMat3(name, m3) } -> std::same_as<void>;
		{ shader.SetMat4(name, m4) } -> std::same_as<void>;
		{ shader.GetName() } -> std::same_as<const std::string&>;
	};
	static_assert(ShaderConcept<Shader>);
}
