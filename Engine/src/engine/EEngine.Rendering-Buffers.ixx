export module EEngine.Rendering:Buffers;
import EEngine.Core;
import EEngine.Standard;

export namespace EEngine {
	// ============================================================================
	// Buffer & Layout (API-agnostic interfaces)
	// ============================================================================

	enum class ShaderData {
		None,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	inline uint32_t ShaderDataSize(ShaderData type) {
		switch (type) {
			case ShaderData::None:		break;
			case ShaderData::Float:		return 4;
			case ShaderData::Float2:		return 4 * 2;
			case ShaderData::Float3:		return 4 * 3;
			case ShaderData::Float4:		return 4 * 4;
			case ShaderData::Mat3:		return 4 * 3 * 3;
			case ShaderData::Mat4:		return 4 * 4 * 4;
			case ShaderData::Int:			return 4;
			case ShaderData::Int2:		return 4 * 2;
			case ShaderData::Int3:		return 4 * 3;
			case ShaderData::Int4:		return 4 * 4;
			case ShaderData::Bool:		return 1;
		}

		Log::CoreError("Unknown shader data type.");
		return 0;
	}

	inline uint32_t ShaderDataComponentCount(ShaderData type) {
		switch (type) {
			case ShaderData::None:		break;
			case ShaderData::Float:		return 1;
			case ShaderData::Float2:		return 2;
			case ShaderData::Float3:		return 3;
			case ShaderData::Float4:		return 4;
			case ShaderData::Mat3:		return 3 * 3;
			case ShaderData::Mat4:		return 4 * 4;
			case ShaderData::Int:			return 1;
			case ShaderData::Int2:		return 2;
			case ShaderData::Int3:		return 3;
			case ShaderData::Int4:		return 4;
			case ShaderData::Bool:		return 1;
		}

		Log::CoreError("Unknown shader data type.");
		return 0;
	}

	struct BufferElement {
		std::string Name;
		ShaderData Type;
		uint32_t Offset;
		uint32_t Size;
		uint32_t ComponentCount;
		bool Normalized;

		BufferElement(
			ShaderData type,
			const std::string& name,
			bool normalized = false
		) : Name(name),
			Type(type),
			Size(ShaderDataSize(type)),
			Offset(0),
			ComponentCount(ShaderDataComponentCount(type)),
			Normalized(normalized) {}
	};

	class BufferLayout {
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements) {
			CalculateOffsetsAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline const uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;

		void CalculateOffsetsAndStride() {
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}
	};

	class IIndexBuffer {
	public:
		virtual ~IIndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetCount() const = 0;
	};

	class IVertexBuffer {
	public:
		virtual ~IVertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
	};
}
