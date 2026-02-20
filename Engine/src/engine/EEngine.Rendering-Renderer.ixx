export module EEngine.Rendering:Renderer;
import EEngine.Core;
import EEngine.Math;
import EEngine.Standard;
import :API;
import :Camera;
import :Interfaces;
import :OpenGL;

using namespace EEngine;

export namespace EEngine {
	// ============================================================================
	// Shader Library
	// ============================================================================

	class ShaderLibrary {
	public:
		void Add(const Shared<IShader>& shader) {
			auto& name = shader->GetName();
			Add(name, shader);
		}

		void Add(const std::string& name, const Shared<IShader>& shader)  {
			Log::CoreAssert(m_ShaderByName.find(name) == m_ShaderByName.end(), "Tried to add duplicate shader.");
			m_ShaderByName[name] = shader;
		}

		Shared<IShader> Load(const std::string& path);

		Shared<IShader> Load(const std::string& name, const std::string& path);

		bool TryGet(const std::string& name, Shared<IShader>* shader) {
			bool foundShader = m_ShaderByName.find(name) != m_ShaderByName.end();
			Log::CoreAssert(foundShader, "Shader not found.");
			if (foundShader) {
				*shader = m_ShaderByName[name];
			}

			return foundShader;
		}
	private:
		std::unordered_map<std::string, Shared<IShader>> m_ShaderByName;
	};
}

// ============================================================================
// Renderer API Facade
// ============================================================================

namespace EEngine::RendererAPI {
	inline IRendererAPI& GetRendererAPI() {
		static OpenGLRendererAPI instance;
		return instance;
	}

	export inline void Initialize() {
		GetRendererAPI().InitializeImpl();
	}

	export inline void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		GetRendererAPI().SetViewportImpl(x, y, width, height);
	}

	export inline void Clear(const Math::vec4& color) {
		GetRendererAPI().ClearImpl(color);
	}

	export inline void DrawIndexed(const Shared<IVertexArray>& vertexArray) {
		GetRendererAPI().DrawIndexedImpl(vertexArray);
	}

	export inline Shared<IIndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count) {
		return GetRendererAPI().CreateIndexBufferImpl(indices, count);
	}

	export inline Shared<IVertexBuffer> CreateVertexBuffer(float* vertices, uint32_t size) {
		return GetRendererAPI().CreateVertexBufferImpl(vertices, size);
	}

	export inline Shared<IShader> CreateShader(
		const std::string& name,
		const std::string& vertexSource,
		const std::string& fragmentSource
	) {
		return GetRendererAPI().CreateShaderImpl(name, vertexSource, fragmentSource);
	}

	export inline Shared<IShader> CreateShader(const std::string& path) {
		return GetRendererAPI().CreateShaderImpl(path);
	}

	export inline Shared<IVertexArray> CreateVertexArray() {
		return GetRendererAPI().CreateVertexArrayImpl();
	}

	export inline Shared<ITexture2D> CreateTexture2D(const std::string& path) {
		return GetRendererAPI().CreateTexture2DImpl(path);
	}

	export inline Shared<ITexture2D> CreateTexture2D(uint32_t width, uint32_t height, void* data = nullptr, uint32_t size = 0) {
		return GetRendererAPI().CreateTexture2DImpl(width, height, data, size);
	}
}

namespace EEngine {
	Shared<IShader> ShaderLibrary::Load(const std::string& path) {
		auto shader = RendererAPI::CreateShader(path);
		Add(shader);
		return shader;
	}

	Shared<IShader> ShaderLibrary::Load(const std::string& name, const std::string& path) {
		auto shader = RendererAPI::CreateShader(path);
		Add(name, shader);
		return shader;
	}
}

// ============================================================================
// High-Level Renderer
// ============================================================================

namespace EEngine::Renderer {
	struct SceneData {
		Math::mat4 ProjectionView;
	};

	inline SceneData& GetSceneData() {
		static SceneData instance;
		return instance;
	}

	inline Shared<ShaderLibrary>& GetShaderLibraryInstance() {
		static Shared<ShaderLibrary> instance = MakeShared<ShaderLibrary>();
		return instance;
	}

	inline API s_SelectedAPI = API::OpenGL;

	export void Initialize() {
		RendererAPI::Initialize();
	}

	export void OnWindowResized(uint32_t width, uint32_t height) {
		RendererAPI::SetViewport(0, 0, width, height);
	}

	export void BeginScene(const Camera& camera) {
		GetSceneData().ProjectionView = camera.GetProjectionViewMatrix();
	}

	export void EndScene() {

	}

	export void Submit(
		const Shared<IShader>& shader,
		const Shared<IVertexArray>& vertexArray,
		const Math::mat4& transform = Math::mat4(1.0f)
	) {
		shader->Bind();
		shader->SetMat4("u_ProjectionView", GetSceneData().ProjectionView);
		shader->SetMat4("u_Transform", transform);
		vertexArray->Bind();
		RendererAPI::DrawIndexed(vertexArray);
	}

	export inline API GetSelectedAPI() { return s_SelectedAPI; }
	export std::string GetRendererAPIString(API api) {
		switch (api) {
			case API::None:
				return "None";
			case API::OpenGL:
				return "OpenGL";
		}

		Log::CoreError("Unknown rendering API.");
		return "";
	}

	export inline Shared<ShaderLibrary> GetShaderLibrary() { return GetShaderLibraryInstance(); }
}
