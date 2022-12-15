export module EEngine.Rendering:Renderer2D;
import :Camera;
import :Buffer;
import :IShader;
import :IVertexArray;
import :OpenGLShader; // ER TODO hackish?
import :Renderer;
import :RendererAPI;
import :ShaderLibrary;
import EEngine.Core;
import EEngine.Math;
import EEngine.std.core;

namespace EEngine::Renderer2D {
	struct Renderer2DData {
		Ref<IVertexArray> QuadVertexArray{};
		Ref<IShader> Shader{};
	} *s_Data ;

	export void Initialize() {
		s_Data = new Renderer2DData();
		s_Data->QuadVertexArray = RendererAPI::CreateVertexArray();

		float vertices[4*3] = {
			-0.5f,	-0.5f,	0.0f,
			0.5f,		-0.5f,	0.0f,
			0.5f,		0.5f,		0.0f,
			-0.5f,	0.5f,		0.0f,
		};

		Ref<IVertexBuffer> vertexBuffer = RendererAPI::CreateVertexBuffer(vertices, sizeof(vertices));
		vertexBuffer->SetLayout({
			{ ShaderData::Float3, "a_Position" }
		});
		s_Data->QuadVertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IIndexBuffer> indexBuffer = RendererAPI::CreateIndexBuffer(indices, sizeof(indices)/sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(indexBuffer);

		s_Data->Shader = Renderer::GetShaderLibrary()->Load("assets/shaders/FlatColor.glsl");
	}

	export void Shutdown() {
		delete s_Data;
	}

	export void BeginScene(const Camera& camera) {
		auto castedShader = std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader);
		castedShader->Bind();
		castedShader->UploadUniformMat4("u_ProjectionView", camera.GetProjectionViewMatrix());
		castedShader->UploadUniformMat4("u_Transform", Math::mat4(1.0f));
	}

	export void EndScene() {

	}

	export void DrawQuad(const Math::vec3& position, const Math::vec2& size, const Math::vec4& color) {
		auto castedShader = std::dynamic_pointer_cast<OpenGLShader>(s_Data->Shader);
		castedShader->Bind();
		castedShader->UploadUniformFloat4("u_Color", color);

		s_Data->QuadVertexArray->Bind();
		RendererAPI::DrawIndexed(s_Data->QuadVertexArray);
	}

	export void DrawQuad(const Math::vec2& position, const Math::vec2& size, const Math::vec4& color) {
		DrawQuad({position.x, position.y, 0.0f }, size, color);
	}
	// primitives
}