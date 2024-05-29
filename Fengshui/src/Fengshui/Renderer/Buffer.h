#pragma once


namespace Fengshui
{
	enum class ShaderDataType : uint8_t
	{
		None = 0, Float, Vec2, Vec3, Vec4, Mat3, Mat4, Int, IntVec2, IntVec3, IntVec4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:   return 4;
			case ShaderDataType::Vec2:    return 4 * 2;
			case ShaderDataType::Vec3:    return 4 * 3;
			case ShaderDataType::Vec4:    return 4 * 4;
			case ShaderDataType::Int:     return 4;
			case ShaderDataType::IntVec2: return 4 * 2;
			case ShaderDataType::IntVec3: return 4 * 3;
			case ShaderDataType::IntVec4: return 4 * 4;
			case ShaderDataType::Mat3:    return 4 * 3 * 3;
			case ShaderDataType::Mat4:    return 4 * 4 * 4;
			case ShaderDataType::Bool:    return 1;
			default:
			{
				FS_ENGINE_ASSERT(false, "Unknown Shader Data Type");
				return 0;
			}
		}
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement() : Name("Empty Element"), Type(ShaderDataType::None), Size(ShaderDataTypeSize(ShaderDataType::None)), Offset(0), Normalized(false) {}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{

		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:   return 1;
				case ShaderDataType::Vec2:    return 2;
				case ShaderDataType::Vec3:    return 3;
				case ShaderDataType::Vec4:    return 4;
				case ShaderDataType::Int:     return 1;
				case ShaderDataType::IntVec2: return 2;
				case ShaderDataType::IntVec3: return 3;
				case ShaderDataType::IntVec4: return 4;
				case ShaderDataType::Mat3:    return 3 * 3;
				case ShaderDataType::Mat4:    return 4 * 4;
				case ShaderDataType::Bool:    return 1;
				default:
				{
					FS_ENGINE_ASSERT(false, "Unknown Shader Data Type");
					return 0;
				}
			}
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout()
		{

		}

		BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		static Ref<VertexBuffer> Create(uint32_t size, float* vertices = nullptr);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		virtual void SetData(const void* data, uint32_t size) = 0;

		static Ref <IndexBuffer> Create(uint32_t count, uint32_t* indices = nullptr);
	};
}