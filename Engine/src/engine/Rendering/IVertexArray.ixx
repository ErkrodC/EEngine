export module EEngine.Rendering:IVertexArray;
import :Buffer;
import EEngine.Core;
import EEngine.Standard;

export namespace EEngine {
	class IVertexArray {
	public:
		virtual ~IVertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetIndexBuffer(const Shared<IIndexBuffer>& indexBuffer) = 0;
		virtual void AddVertexBuffer(const Shared<IVertexBuffer>& vertexBuffer) = 0;

		virtual const Shared<IIndexBuffer>& GetIndexBuffer() const = 0;
		virtual const std::vector<Shared<IVertexBuffer>>& GetVertexBuffers() const = 0;
	};
} // EEngine
