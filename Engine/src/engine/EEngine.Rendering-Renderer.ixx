export module EEngine.Rendering:Renderer;
import EEngine.Core;
import EEngine.Standard;
import :API;
import :Camera;
import :Interfaces;

using namespace EEngine;

namespace EEngine::Renderer {
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

		Shared<IShader> Load(const std::string& path) {
			auto shader = RendererAPI::CreateShader(path);
			Add(shader);
			return shader;
		}

		Shared<IShader> Load(const std::string& name, const std::string& path) {
			auto shader = RendererAPI::CreateShader(path);
			Add(name, shader);
			return shader;
		}

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

	enum class API {
		None = 0,
		OpenGL = 1,
	};

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

	export {
		void Initialize() {
			RendererAPI::Initialize();
		}

		void OnWindowResized(uint32_t width, uint32_t height) {
			RendererAPI::SetViewport(0, 0, width, height);
		}

		void BeginScene(const Camera& camera) {
			GetSceneData().ProjectionView = camera.GetProjectionViewMatrix();
		}

		void EndScene() {

		}

		void Submit(
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

		inline API GetSelectedAPI() { return s_SelectedAPI; }

		std::string GetRendererAPIString(API api) {
			switch (api) {
				case API::None:
					return "None";
				case API::OpenGL:
					return "OpenGL";
			}

			Log::CoreError("Unknown rendering API.");
			return "";
		}

		inline Shared<ShaderLibrary> GetShaderLibrary() { return GetShaderLibraryInstance(); }
	}
}
