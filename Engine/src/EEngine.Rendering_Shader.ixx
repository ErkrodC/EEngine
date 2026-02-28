export module EEngine.Rendering:Shader;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;
import :API;

export namespace EEngine::Rendering {
	// ============================================================================
	// OpenGLShader Implementation
	// ============================================================================
	class OpenGLShader {
	public:
		OpenGLShader(
			const std::string& name,
			const std::string& vertexSource,
			const std::string& fragmentSource
		);

		explicit OpenGLShader(const std::string& path);

		~OpenGLShader();
		OpenGLShader(const OpenGLShader& other) = delete;
		OpenGLShader& operator=(const OpenGLShader& other) = delete;

		OpenGLShader(OpenGLShader&& other) noexcept;
		OpenGLShader& operator=(OpenGLShader&& other) noexcept;

		void Bind() const;
		void Unbind() const;

		void SetInt(const std::string& name, int32_t value);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const Math::vec2& values);
		void SetFloat3(const std::string& name, const Math::vec3& values);
		void SetFloat4(const std::string& name, const Math::vec4& values);
		void SetMat3(const std::string& name, const Math::mat3& matrix);
		void SetMat4(const std::string& name, const Math::mat4& matrix);

		const std::string& GetName() const { return m_Name; }
	private:
		uint32_t m_RendererID;
		std::string m_Name;
		mutable std::unordered_map<std::string, int32_t> m_UniformLocationCache;

		static void IndentLog(std::vector<char>& log);
		static bool TryCompileShader(uint32_t shaderType, const std::string& source, uint32_t* compiledShaderID);
		std::string ReadFile(const std::string& path);
		static uint32_t ShaderTypeFromString(const std::string& type);
		std::unordered_map<uint32_t, std::string> Preprocess(const std::string& source);
		void CompileShaders(const std::unordered_map<uint32_t, std::string>& shaderSourceByType);
		int32_t GetUniformLocation(const std::string& name) const;
	};

	// ============================================================================
	// Shader Type Alias
	// ============================================================================
	class DirectXShader; 
	class VulkanShader; 
	using Shader = std::conditional_t<g_API == API::DirectX, 
		DirectXShader, 
		std::conditional_t<g_API == API::OpenGL, 
			OpenGLShader, 
			VulkanShader 
		> 
	>;

	// ============================================================================
	// Shader Concept and Static Assert
	// ============================================================================
	template<typename T>
	concept ShaderConcept = requires(T shader, std::string name, int32_t i, float f, Math::vec2 v2, Math::vec3 v3, Math::vec4 v4, Math::mat3 m3, Math::mat4 m4) {
		{ shader.Bind() } -> std::same_as<void>;
		{ shader.Unbind() } -> std::same_as<void>;
		{ shader.SetInt(name, i) } -> std::same_as<void>;
		{ shader.SetFloat(name, f) } -> std::same_as<void>;
		{ shader.SetFloat2(name, v2) } -> std::same_as<void>;
		{ shader.SetFloat3(name, v3) } -> std::same_as<void>;
		{ shader.SetFloat4(name, v4) } -> std::same_as<void>;
		{ shader.SetMat3(name, m3) } -> std::same_as<void>;
		{ shader.SetMat4(name, m4) } -> std::same_as<void>;
		{ shader.GetName() } -> std::same_as<const std::string&>;
	};
	static_assert(ShaderConcept<Shader>);
}
