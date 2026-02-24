export module EEngine.Rendering:Renderer;
import EEngine.Core;
import EEngine.Standard;
import :API;
import :Camera;
import :Interfaces;

using namespace EEngine;

namespace EEngine::Renderer {
	using namespace RendererAPI;

	class ShaderLibrary {
	public:
		void Add(const Shared<Shader>& shader) {
			auto& name = shader->GetName();
			Add(name, shader);
		}

		void Add(const std::string& name, const Shared<Shader>& shader)  {
			Log::CoreAssert(!m_ShaderByName.contains(name), "Tried to add duplicate shader.");
			m_ShaderByName[name] = shader;
		}

		Shared<Shader> Load(const std::string& path) {
			auto shader = CreateShader(path);
			Add(shader);
			return shader;
		}

		Shared<Shader> Load(const std::string& name, const std::string& path) {
			auto shader = CreateShader(path);
			Add(name, shader);
			return shader;
		}

		bool TryGet(const std::string& name, Shared<Shader>& outShader) {
			bool foundShader = m_ShaderByName.contains(name);
			Log::CoreAssert(foundShader, "Shader not found.");
			if (foundShader) {
				outShader = m_ShaderByName[name];
			}

			return foundShader;
		}
	private:
		std::unordered_map<std::string, Shared<Shader>> m_ShaderByName;
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

	export {
		void Initialize() {
			RendererAPI::Initialize();
		}

		void OnWindowResized(uint32_t width, uint32_t height) {
			SetViewport(0, 0, width, height);
		}

		void BeginScene(const Camera& camera) {
			GetSceneData().ProjectionView = camera.GetProjectionViewMatrix();
		}

		void EndScene() {

		}

		void Submit(
			const Shared<Shader>& shader,
			const Shared<VertexArray>& vertexArray,
			const Math::mat4& transform = Math::mat4(1.0f)
		) {
			shader->Bind();
			shader->SetMat4("u_ProjectionView", GetSceneData().ProjectionView);
			shader->SetMat4("u_Transform", transform);
			vertexArray->Bind();
			DrawIndexed(vertexArray);
		}

		inline Shared<ShaderLibrary> GetShaderLibrary() { return GetShaderLibraryInstance(); }
	}
}
